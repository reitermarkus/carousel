[![](https://api.travis-ci.org/reitermarkus/carousel.svg)](https://travis-ci.org/reitermarkus/carousel)


# Installation

Compile with

```bash
make
```

and run using

```bash
make run
```


# Camera Controls

The program starts with the camera in automatic mode, when a
direction key is pressed, it will change to manual mode, to
switch back to automatic mode, press the delete key.

| Key      | Command                    |
|----------|----------------------------|
| Q        | rotate left                |
| E        | rotate right               |
| W        | move forward               |
| S        | move backward              |
| A        | move left                  |
| D        | move right                 |
| R        | move up                    |
| F        | move down                  |
| H        | brighten ambient color     |
| J        | darken ambient color       |
| K        | change light hue (-)       |
| L        | change light hue (+)       |
| I        | toggle light saturation    |
| 1        | toggle light 1             |
| 2        | toggle light 2             |
| 6        | toggle ambient light       |
| 7        | toggle diffuse light       |
| 8        | toggle specular light      |
| spacebar | show wireframe model       |
| delete   | reset camera to auto-mode  |


# Mouse Controls

| Gesture         | Command              |
|-----------------|----------------------|
| drag up/down    | rotate around x-axis |
| drag left/right | rotate around y-axis |
