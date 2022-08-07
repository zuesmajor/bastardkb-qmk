#! /usr/bin/env python3

import argparse
import logging
import os
import os.path
import re
import shlex
import signal
import subprocess
import sys
import tempfile

from collections.abc import Callable, Sequence
from functools import partial, reduce
from logging.handlers import RotatingFileHandler
from operator import iconcat
from pathlib import Path, PurePath
from pygit2 import (
    GitError,
    Repository,
)
from rich.console import Console, Group
from rich.live import Live
from rich.progress import (
    BarColumn,
    MofNCompleteColumn,
    Progress,
    SpinnerColumn,
    TextColumn,
    TimeElapsedColumn,
)
from typing import NamedTuple, Optional


class Firmware(NamedTuple):
    keyboard: str
    keymap: str
    keymap_alias: Optional[str] = None
    env_vars: Sequence[str] = []

    @property
    def output_filename(self) -> str:
        return f"bastardkb_{self.keyboard.replace('/', '_')}_{self.keymap_alias or self.keymap}"

    def __str__(self) -> str:
        return f"{self.keyboard}:{self.keymap}"


class FirmwareList(NamedTuple):
    branch: str
    configurations: Sequence[Firmware]


NON_CHARYBDIS_KEYBOARDS: Sequence[str] = (
    "skeletyl",
    "tbkmini",
    "scylla",
)
CHARYBDIS_KEYBOARDS: Sequence[str] = (
    "charybdis/3x5",
    "charybdis/3x6",
    "charybdis/4x6",
)
ALL_BASTARD_KEYBOARDS: Sequence[str] = (
    *NON_CHARYBDIS_KEYBOARDS,
    *CHARYBDIS_KEYBOARDS,
)

SMALL_FOOTPRINT_ADAPTERS: Sequence[str] = (
    "v1/elitec",
    "v2/elitec",
)

STABLE_ADAPTERS: Sequence[str] = (
    *SMALL_FOOTPRINT_ADAPTERS,
    "blackpill",
)

ALL_ADAPTERS: Sequence[str] = (
    *STABLE_ADAPTERS,
    "v2/stemcell",
    "v2/splinky",
)

ALL_FIRMWARES: Sequence[FirmwareList] = (
    # All firmwares built on the `bkb-develop` branch, ie. the branch tracking
    # `qmk/qmk_firmware:develop`.
    FirmwareList(
        branch="bkb-develop",
        configurations=(
            # Use the `default` keymap for the non-Charybdis boards (ie. Scylla,
            # TBK mini, Skeletyl).  These board don't have a `via` keymap and
            # their "stock" configuration is using the `default` keymap instead.
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/{adapter}",
                    keymap="default",
                    keymap_alias="stock",
                )
                for keyboard in NON_CHARYBDIS_KEYBOARDS
                for adapter in STABLE_ADAPTERS
            ),
            # Use the `via` keymap for the Charybdis boards (ie. the Charybdis,
            # Charybdis mini, and Charybdis nano).  These boards have a very
            # bare `default` keymap, and their "stock" configuration is using
            # the `via` keymap instead.
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/{adapter}",
                    keymap="via",
                    keymap_alias="stock",
                )
                for keyboard in CHARYBDIS_KEYBOARDS
                for adapter in STABLE_ADAPTERS
            ),
            # Also build the Blackpill firmwares in uf2 format.
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/blackpill",
                    keymap="default",
                    keymap_alias="stock",
                    env_vars=("BOOTLOADER=tinyuf2",),
                )
                for keyboard in NON_CHARYBDIS_KEYBOARDS
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/blackpill",
                    keymap="via",
                    keymap_alias="stock",
                    env_vars=("BOOTLOADER=tinyuf2",),
                )
                for keyboard in CHARYBDIS_KEYBOARDS
            ),
            *tuple(
                Firmware(
                    keyboard=f"skeletyl/{adapter}",
                    keymap="manna-harbour_miryoku",
                    keymap_alias="miryoku",
                    env_vars=(
                        "MIRYOKU_ALPHAS=QWERTY",
                        "MIRYOKU_EXTRA=COLEMAKDH",
                    ),
                )
                for adapter in STABLE_ADAPTERS
            ),
            Firmware(
                keyboard=f"skeletyl/blackpill",
                keymap="manna-harbour_miryoku",
                keymap_alias="miryoku",
                env_vars=("BOOTLOADER=tinyuf2",),
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/v2/splinky",
                    keymap="default",
                    keymap_alias="stock",
                )
                for keyboard in ALL_BASTARD_KEYBOARDS
            ),
            Firmware(keyboard="dilemma/assembled", keymap="via", keymap_alias="stock"),
            Firmware(keyboard="dilemma/splinky", keymap="via", keymap_alias="stock"),
        ),
    ),
    # All firmwares built on the `bkb-vial` branch, ie. the branch tracking
    # `vial-kb/vial-qmk:vial`.
    FirmwareList(
        branch="bkb-vial",
        configurations=(
            *tuple(
                Firmware(keyboard=f"{keyboard}/{adapter}", keymap="vial")
                for keyboard in ALL_BASTARD_KEYBOARDS
                for adapter in STABLE_ADAPTERS
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/blackpill",
                    keymap="vial",
                    env_vars=("BOOTLOADER=tinyuf2",),
                )
                for keyboard in ALL_BASTARD_KEYBOARDS
            ),
        ),
    ),
    # All firmwares built on the `bkb-develop-feat-zykrah-vial` branch, ie. the
    # branch tracking `zykrah/vial-qmk:vial-develop`.
    FirmwareList(
        branch="bkb-develop-feat-zykrah-vial",
        configurations=(
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/{adapter}",
                    keymap="vial",
                    keymap_alias="vial-develop",
                )
                for keyboard in ALL_BASTARD_KEYBOARDS
                for adapter in STABLE_ADAPTERS
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/blackpill",
                    keymap="vial",
                    keymap_alias="vial-develop",
                    env_vars=("BOOTLOADER=tinyuf2",),
                )
                for keyboard in ALL_BASTARD_KEYBOARDS
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/v2/splinky",
                    keymap="vial",
                    keymap_alias="vial-develop",
                )
                for keyboard in ALL_BASTARD_KEYBOARDS
            ),
            Firmware(keyboard="dilemma/assembled", keymap="vial", keymap_alias="vial-develop"),
            Firmware(keyboard="dilemma/splinky", keymap="vial", keymap_alias="vial-develop"),
        ),
    ),
    # Firmware build off the `bkb-vial-feat-miryoku` branch that contains quick
    # and dirty changes to build a Vial-enabled Miryoku keymap for the Skeletyl.
    FirmwareList(
        branch="bkb-vial-feat-manna_harbour-miryoku",
        configurations=(
            *tuple(
                Firmware(
                    keyboard=f"skeletyl/{adapter}",
                    keymap="manna-harbour_miryoku",
                    keymap_alias="miryoku-vial",
                    env_vars=(
                        "KEY_OVERRIDE_ENABLE=no",
                        "LTO_ENABLE=yes",
                        "MIRYOKU_ALPHAS=QWERTY",
                        "MIRYOKU_EXTRA=COLEMAKDH",
                        "QMK_SETTINGS=no",
                        "SPACE_CADET_ENABLE=no",
                        "TAP_DANCE_ENABLE=no",
                        "VIALRGB_ENABLE=yes",
                        "VIAL_ENABLE=yes",
                        "VIAL_INSECURE=yes",
                        "VIA_ENABLE=yes",
                    ),
                )
                for adapter in SMALL_FOOTPRINT_ADAPTERS
            ),
            Firmware(
                keyboard="skeletyl/blackpill",
                keymap="manna-harbour_miryoku",
                keymap_alias="miryoku-vial",
                env_vars=(
                    "LTO_ENABLE=no",
                    "MIRYOKU_ALPHAS=QWERTY",
                    "MIRYOKU_EXTRA=COLEMAKDH",
                    "VIA_ENABLE=yes",
                    "VIAL_ENABLE=yes",
                    "VIAL_INSECURE=yes",
                    "VIALRGB_ENABLE=yes",
                ),
            ),
            Firmware(
                keyboard="skeletyl/blackpill",
                keymap="manna-harbour_miryoku",
                keymap_alias="miryoku-vial",
                env_vars=(
                    "BOOTLOADER=tinyuf2",
                    "LTO_ENABLE=no",
                    "MIRYOKU_ALPHAS=QWERTY",
                    "MIRYOKU_EXTRA=COLEMAKDH",
                    "VIA_ENABLE=yes",
                    "VIAL_ENABLE=yes",
                    "VIAL_INSECURE=yes",
                    "VIALRGB_ENABLE=yes",
                ),
            ),
        ),
    ),
    # All firmwares built on the `bkb-stemcell` branch, ie. the branch tracking
    # `qmk/qmk_firmware:pull/16287/head` (not merged yet).
    # See https://github.com/qmk/qmk_firmware/pull/16287.
    FirmwareList(
        branch="bkb-stemcell",
        configurations=(
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/v2/stemcell",
                    keymap="default",
                    keymap_alias="stock",
                    env_vars=("STMC=yes", "STMC_US=yes"),
                )
                for keyboard in NON_CHARYBDIS_KEYBOARDS
            ),
            *tuple(
                Firmware(
                    keyboard=f"{keyboard}/v2/stemcell",
                    keymap="via",
                    keymap_alias="stock",
                    env_vars=("STMC=yes", "STMC_US=yes"),
                )
                for keyboard in CHARYBDIS_KEYBOARDS
            ),
        ),
    ),
)


class Reporter(object):
    def __init__(self, verbose: bool):
        self.console = Console()
        self.logging = logging.getLogger()
        self.verbose = verbose

        # Logging setup.
        logging_file_handler = RotatingFileHandler(
            filename=os.path.join(os.getcwd(), f"{os.path.basename(__file__)}.log"),
            encoding="utf-8",
            maxBytes=1024 * 1024,
            backupCount=5,
        )
        logging_file_handler.setFormatter(logging.Formatter(fmt="%(asctime)s %(levelname)s %(message)s"))
        self.logging.addHandler(logging_file_handler)
        self.logging.setLevel(level=logging.DEBUG)

        self.log_dir = tempfile.mkdtemp()
        self.debug(f"Saving logs in: {self.log_dir}")

        # Progress status.
        self._progress_status = lambda _: None

    def log_file(self, basename: str) -> Path:
        return Path(self.log_dir, basename).with_suffix(".log")

    def set_progress_status(self, progress_status: Callable[[str], None]) -> None:
        self._progress_status = progress_status

    def progress_status(self, message):
        self._progress_status(message)
        self.logging.info(message)

    def newline(self):
        self.console.print("")

    def debug(self, message) -> None:
        self.logging.debug(message)

    def info(self, message, **kwargs) -> None:
        self.console.print(message, **kwargs)
        self.logging.info(message)

    def warn(self, message, **kwargs) -> None:
        self.console.print(message, **kwargs)
        self.logging.warning(message)

    def error(self, message) -> None:
        self.console.print(message)
        self.logging.error(message)


class QmkCompletedProcess(object):
    def __init__(self, completed_process: subprocess.CompletedProcess, log_file: Path):
        self._completed_process = completed_process
        self.log_file = log_file

    @property
    def returncode(self) -> int:
        return self._completed_process.returncode


class Executor(object):
    def __init__(self, reporter: Reporter, repository: Repository, dry_run: bool):
        self.dry_run = dry_run
        self.reporter = reporter
        self.repository = repository

    def git_checkout(self, branch: str, update_submodules: bool) -> None:
        self.reporter.progress_status(f"Checking out [bright_magenta]{branch}[/bright_magenta]…")
        try:
            branch_ref = self.repository.branches[branch]
        except KeyError:
            self.reporter.error("Branch does not exist")
            sys.exit(1)
        if not self.dry_run:
            self.repository.checkout(branch_ref)
            if update_submodules:
                self.reporter.progress_status(f"([bright_magenta]{branch}[/bright_magenta]) Updating submodules…")
                # TODO(0xcharly): use pygit2 to update submodules.
                self._run(
                    ("git", "submodule", "update", "--init", "--recursive"),
                    log_file=self.reporter.log_file(f"git-submodule-update-{branch}"),
                    cwd=self.repository.workdir,
                )
        else:
            self.reporter.progress_status(f"([bright_magenta]{branch}[/bright_magenta]) Updating submodules…")

    def qmk_compile(self, firmware: Firmware) -> QmkCompletedProcess:
        self.reporter.progress_status(f"Compiling [bold white]{firmware}[/bold white]")
        argv = (
            "qmk",
            "compile",
            "--clean",
            "--parallel",
            "0",
            "--keyboard",
            f"bastardkb/{firmware.keyboard}",
            "--keymap",
            firmware.keymap,
            "--env",
            f"TARGET={firmware.output_filename}",
            # "--env",
            # f"USE_CCACHE=yes",
            *reduce(iconcat, (("-e", env_var) for env_var in firmware.env_vars), []),
        )
        log_file = self.reporter.log_file(f"qmk-compile-{firmware.output_filename}")
        return QmkCompletedProcess(self._run(argv, log_file=log_file), log_file)

    def _run(
        self,
        argv: Sequence[str],
        log_file: Path,
        **kwargs,
    ) -> subprocess.CompletedProcess:
        self.reporter.debug(f"exec: {shlex.join(argv)}")
        self.reporter.debug(f"output: {log_file}")
        if not self.dry_run:
            with log_file.open("w") as fd:
                kwargs["stdout"] = fd
                kwargs["stderr"] = fd
                return subprocess.run(argv, **kwargs)
        return subprocess.CompletedProcess(args=argv, returncode=0)


def total_firmware_count_reduce_callback(acc: int, firmware_list: FirmwareList) -> int:
    return acc + len(list(firmware_list.configurations))


def read_firmware_filename_from_logs(firmware: Firmware, log_file: Path) -> Path:
    pattern = re.compile(
        f"Copying (?P<filename>{re.escape(firmware.output_filename)}\\.[a-z0-9]+) to qmk_firmware folder"
    )
    with log_file.open() as fd:
        for line in fd:
            match = pattern.match(line)
            if match:
                return Path(match.group("filename"))
    raise FileNotFoundError()


def build(
    executor: Executor,
    reporter: Reporter,
    firmwares: Sequence[FirmwareList],
    on_firmware_compiled: Callable[[Path], None],
) -> None:
    empty_status = Progress(TextColumn(""))
    overall_status = Progress(SpinnerColumn(), TextColumn("{task.description}"))
    overall_progress = Progress(
        MofNCompleteColumn(),
        BarColumn(complete_style="blue"),
        TextColumn("[magenta]{task.percentage:>5.1f}%"),
        TimeElapsedColumn(),
        console=reporter.console,
    )
    progress_group = Group(empty_status, overall_status, overall_progress)

    total_firmware_count = reduce(total_firmware_count_reduce_callback, firmwares, 0)
    built_firmware_count = 0
    newline_task = empty_status.add_task("")
    overall_status_task = overall_status.add_task("Preparing…")
    overall_progress_task = overall_progress.add_task("", total=total_firmware_count)
    reporter.set_progress_status(lambda message: overall_status.update(overall_status_task, description=message))
    reporter.info(f"Preparing to build {total_firmware_count} BastardKB firmwares")
    with Live(progress_group, console=reporter.console):
        for branch, configurations in firmwares:
            # Checkout branch.
            reporter.info(f"  Building off branch [magenta]{branch}[/magenta] ({len(configurations)} firmwares)")
            executor.git_checkout(branch, update_submodules=True)

            # Build firmwares off that branch.
            for firmware in configurations:
                completed_process = executor.qmk_compile(firmware)
                if completed_process.returncode == 0:
                    try:
                        on_firmware_compiled(read_firmware_filename_from_logs(firmware, completed_process.log_file))
                        built_firmware_count += 1
                        reporter.info(
                            f"    CC [not bold white]{str(firmware):46}[/not bold white] [green]SUCCESS[/green]"
                        )
                    except FileNotFoundError:
                        reporter.warn(
                            f"    CC [not bold white]{str(firmware):46}[/not bold white] [yellow]WARNING[/yellow]"
                        )
                else:
                    reporter.error(f"    CC [not bold white]{str(firmware):46}[/not bold white] [red]FAILURE[/red]")
                overall_progress.update(overall_progress_task, advance=1)
            reporter.newline()
        overall_status.update(overall_status_task, visible=False)
        empty_status.update(newline_task, visible=False)
        reporter.info(f"Done: built={built_firmware_count}, failed={total_firmware_count - built_firmware_count}")


def copy_firmware_to_output_dir(reporter: Reporter, output_dir: Path, repository_path: Path, firmware_filename: Path):
    try:
        firmware_file = repository_path / firmware_filename
        target = output_dir / firmware_file.name
        if firmware_file != target:
            reporter.logging.debug(f"copy: {firmware_file} -> {target}")
            firmware_file.rename(target)
        else:
            reporter.logging.debug(f"firmware already at {firmware_file}")
    except OSError:
        reporter.logging.exception("failed to copy firmware to output directory")


def sigint_handler(reporter: Reporter, signal, frame):
    del signal, frame
    reporter.progress_status("Interrupted.  Exiting…")
    sys.exit(1)


def main() -> None:
    # Parse command line arguments.
    parser = argparse.ArgumentParser(description="Create Bastard Keyboard firmware release.")
    parser.add_argument(
        "-n",
        "--dry-run",
        action="store_true",
        help="Don't actually build, just show the commands to be run.",
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output.")
    parser.add_argument(
        "-r",
        "--repository",
        type=PurePath,
        help="The QMK repository checkout to work with",
        default=Path.cwd(),
    )
    parser.add_argument(
        "-o",
        "--output-dir",
        type=Path,
        help="The output directory in which to copy the artifacts",
        default=Path.cwd(),
    )
    cmdline_args = parser.parse_args()
    reporter = Reporter(cmdline_args.verbose)

    # Install SIGINT handler.
    signal.signal(signal.SIGINT, partial(sigint_handler, reporter))

    # Open QMK repository.
    try:
        repository = Repository(cmdline_args.repository)
    except GitError:
        reporter.error("Failed to initialize QMK repository")
        sys.exit(1)

    # Create output dir if needed.
    try:
        cmdline_args.output_dir.mkdir(parents=True, exist_ok=True)
    except FileExistsError:
        reporter.error("Output path exists and is not a directory")
        sys.exit(1)

    executor = Executor(reporter, repository, cmdline_args.dry_run)
    build(
        executor,
        reporter,
        ALL_FIRMWARES,
        partial(
            copy_firmware_to_output_dir,
            reporter,
            cmdline_args.output_dir,
            cmdline_args.repository,
        ),
    )  # Build firmwares.


if __name__ == "__main__":
    main()
