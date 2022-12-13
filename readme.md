# Bastard Keyboards firmwares

This repository contains the firmware code and releases for the Bastard Keyboards Charybdis boards, including builds for Elite-C, Blackpill, STeMCell, and RP2040-based controllers implementing the BKB standard pinout (_eg._ Splinky, Elite-Pi, etc…).

While this repository is used as primary source of truth for Bastard Keyboards firmwares, as it contains the latest changes and improvements, the maintainers aim at upstreaming all those changes to the official [QMK repository](https://github.com/qmk/qmk_firmware).

# Getting Started

The following section will guide you through downloading and installing the proper firmware on your Bastard Keyboard.

## Releases

The latest releases of the firmware are available in the [Releases](https://github.com/Bastardkb/bastardkb-qmk/releases/) section.

The format of the firmware file name is:

```
bastardkb_{keyboard}_{mcu-adapter}_{keymap}
```

### Keyboard

`{keyboard}` corresponds to the physical keyboard you are looking the firmware for. It can be one of the following:

- `skeletyl`: the 3x5+3 [Skeletyl](https://bastardkb.com/skeletyl/)
- `tbkmini`: the 3x6+3 [TBK Mini](https://bastardkb.com/tbk-mini/)
- `scylla`: the 4x6+5 [Scylla](https://bastardkb.com/scylla/)
- `charybdis_3x5`: the 3x5+3 [Charybdis Nano](https://bastardkb.com/charybdis-nano/)
- `charybdis_3x6`: the 3x6+3 Charybdis Mini
- `charybdis_4x6`: the 4x6+5 [Charybdis](https://bastardkb.com/charybdis/)
- `dilemma`: the 3x5+2 [Dilemma](https://bastardkb.com/dilemma/)

### Microcontroller/adapter

`{mcu-adapter}` corresponds to the microcontroller (_eg._ Elite-C, Blackpill, …) that is installed in your keyboard, and optionally the revision of the adapter. It can be one of the following:

- `v1_elitec`
- `v2_elitec`
- `v2_splinky_v2`
- `v2_splinky_v3`
- `v2_stemcell`
- `blackpill`

The version of the adapter can also be checked directly on the adapter PCB.

If you have any doubts, feel free to reach out for help on the [Discord](https://bastardkb.com/discord).

### Keymap

`{keymap}` corresponds to the keymap you want to use on your keyboard. It can be one of the following:

- `default`
- `vial`

Note that, in most cases, you will want to use the `vial` keymap. The `default` keymap is very barebones, and is meant to be used as a template to start developing your own.

The `vial` keymap is compatible out-of-the-box with [Vial](https://get.vial.today/).

### Extension

The file extension is either `.hex`, `.bin` or `.uf2`, depends on the microcontroller, and is irrelevant for this guide.

## Installing the keymap

The process of installing the keymap on the keyboard is called "flashing". There's various ways to flash a firmware file onto a keyboard, one of which is to use [QMK Toolbox](https://github.com/qmk/qmk_toolbox).

To flash your keyboard:

1. Download [QMK Toolbox](https://github.com/qmk/qmk_toolbox/releases), and install the application
1. Download the firmware file (see [Releases](#releases) section)
1. Disconnect your keyboard (both host-to-keyboard USB cable, and keyboard halves TRS/TRRS cable)
1. Open QMK Toolbox, and select the firmware you've just downloaded (see the [QMK Toolbox documentation](https://github.com/qmk/qmk_toolbox) for more details)
1. Plug in one half of the keyboards
1. Press the `reset` button on your controller
1. Click "Flash" to start the flashing process. **Do not disconnect** the keyboard until the process is complete.
1. Disconnect the first half, and connect the other one
1. Repeat the steps to flash the second half
1. Disconnect the keyboard

The new firmware is now installed on both halves. Close QMK Toolbox, connect the two halves with the TRS/TRRS cable, and connect one half to the PC/Mac.

If you have any questions, feel free to reach out for help on the [Discord](https://bastardkb.com/discord).

## Vial

[Vial](https://get.vial.today/) is an open-source cross-platform (Windows, Linux and Mac) GUI and a QMK fork for configuring your keyboard in real time.

![](https://raw.githubusercontent.com/BastardKb/bastardkb-qmk/main/assets/vial.png)

It is compatible with the `vial` keymaps that can be downloaded from the [Releases](https://github.com/Bastardkb/bastardkb-qmk/releases/) section.

Steps to use Vial:

1. [Download Vial](https://get.vial.today/download) from their website, install and open the application
1. Plug your keyboard in
1. PC/Mac will detect the keyboard automatically
1. Use the Vial interface to edit your keymap. Changes are saved automatically.

If you have any questions, feel free to reach out for help on the [Discord](https://bastardkb.com/discord).

# Going further with QMK

The following section is dedicated to advance users that want to customize their keyboard and keymap by building their own firmware. It contains information pertaining to the firmware development.

If you have any questions, feel free to reach out for help on the [Discord](https://bastardkb.com/discord).

## Building from source (advanced)

Building from source is useful to people who want to customize their keyboard and keymaps beyond what Vial offers. This involves using a command-line interface, writing C code, and compiling this code into the final firmware file.

To build the firmware from source:

1. Follow the [QMK docs](https://docs.qmk.fm/#/getting_started_build_tools) to setup your environment
1. Checkout this repository

To build a firmware **without** Vial support, use the `bkb-master` branch:

```shell
git checkout origin/bkb-master
qmk compile -c -kb {keyboard-arg} -km default
```

To build a firmware **with** Vial support, use the `bkb-vial` branch:

```shell
git checkout origin/bkb-vial
qmk compile -c -kb {keyboard-arg} -km vial
```

See [the `{keyboard-arg}` section](#keyboard-arg) for possible values.

To flash the firmware from the command-line, replace `compile` with `flash` in the commands above:

```shell
qmk flash -c -kb bastardkb/{keyboard} -km default # On the bkb-master branch
```

```shell
qmk flash -c -kb bastardkb/{keyboard} -km vial # On the bkb-vial branch
```

After compilation, QMK waits for the keyboard to become available as a dfu device to upload firmware. Press the `reset` button on the keyboard to complete the process.

### `{keyboard-arg}`

The `{keyboard-arg}` depends on the keyboard you are building the firmware for (_eg._ Skeletyl, TBK-Mini, etc…) and the controller you are targetting (_eg._ Elite-C, Blackpill, etc…).

The `{keyboard-arg}` format is:

```
bastardkb/{keyboard}/{mcu-adapter}
```

### `{keyboard}`

`{keyboard}` corresponds to the physical keyboard you are building the firmware for. It can be one of the following:

- `skeletyl`: the 3x5+3 [Skeletyl](https://bastardkb.com/skeletyl/)
- `tbkmini`: the 3x6+3 [TBK Mini](https://bastardkb.com/tbk-mini/)
- `scylla`: the 4x6+5 [Scylla](https://bastardkb.com/scylla/)
- `charybdis/3x5`: the 3x5+3 [Charybdis Nano](https://bastardkb.com/charybdis-nano/)
- `charybdis/3x6`: the 3x6+3 Charybdis Mini
- `charybdis/4x6`: the 4x6+5 [Charybdis](https://bastardkb.com/charybdis/)
- `dilemma/3x5_2`: the 3x5+2 [Dilemma](https://bastardkb.com/dilemma/)
- `dilemma/3x5_3`: the 3x5+3 [Dilemma](https://bastardkb.com/dilemma/)

### Microcontroller/adapter

`{mcu-adapter}` corresponds to the microcontroller (_eg._ Elite-C, Blackpill, …) that is installed in your keyboard, and optionally the revision of the adapter.

For **the Dilemma 3x5_2**, it can be one of the following:

- `splinky`: for the [DIY version of the Dilemma](https://github.com/bastardkb/dilemma)
- `assembled`: for the [pre-assembled version of the Dilemma](https://bastardkb.com/dilemma/)

For **all other boards**, it can be one of the following:

- `v1/elitec`
- `v2/elitec`
- `v2/splinky_2`
- `v2/splinky_3`
- `v2/stemcell`
- `blackpill`

The version of the adapter can also be checked directly on the adapter PCB.

If you have any doubts, feel free to reach out for help on the [Discord](https://bastardkb.com/discord).

### Why `bkb-master` and `bkb-vial`

The changes on `bkb-master` are meant to be upstreamed and merged into QMK's `master` branch.

Vial, however, does not work out-of-the-box when using QMK's `master` branch, and relies instead of some changes that have not been upstreamed yet. Because of this, this repository trackes Vial's `master` branch (see below, `qmk-vial-head`) and cherry-picks Bastard Keyboards related changes on top of it.

Note that Vial's `master` usually lags a bit behind QMK's `master`, so the latest changes to QMK might not be available when working on `bkb-vial`.

## Feature branches

| Branch following QMK's `master` | Parent                     | Description                                          |
| ------------------------------- | -------------------------- | ---------------------------------------------------- |
| `bkb-master`                    | `qmk/qmk_firmware/master`  | Contains the latest sources for BastardKB's firmware |

| Branch following Vial's `master` | Parent          | Description                                        |
| -------------------------------- | ----------------| -------------------------------------------------- |
| `bkb-vial`                       | `qmk-vial-head` | Vial support on top of the changes in `bkb-master` |
