Turing machine terminal visualizer for the 5-state Busy Beaver.

![Made with VHS](https://vhs.charm.sh/vhs-38F5XCchpxlRqsKO24buf1.gif)

## Build & Run

```
cc -o bb5 bb5.c
./bb5
```

## Options

You might want to change interval if you don't plan to wait a week :)

- `INTERVAL` — render every N steps (default: 1)
- `SLEEP_US` — microseconds between frames (default: 10000)

```
cc -DINTERVAL=1000 -DSLEEP_US=5000 -o bb5 bb5.c
```
