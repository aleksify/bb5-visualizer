Terminal visualizer for the 5-state Busy Beaver champion.

## Build & Run

```
cc -o bb5 bb5.c
./bb5
```

## Options

- `INTERVAL` — render every N steps (default: 1)
- `SLEEP_US` — microseconds between frames (default: 10000)

```
cc -DINTERVAL=1000 -DSLEEP_US=5000 -o bb5 bb5.c
```
