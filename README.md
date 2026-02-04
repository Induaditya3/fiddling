# fiddling

A small grab-bag of standalone programs and learning experiments (mostly Python + C, with a bit of OCaml). Most files are meant to be run directly from the command line.

## Layout

- Repo root: one-off scripts and small programs
- [compile/](compile/): notes + examples on the C/C++ compilation pipeline (has its own README and diagram)
- [daysOfC/](daysOfC/): C learning notes + small demos (has its own README)

## Highlights (worth a look)

- [maze.py](maze.py): maze solver using graph search; can also render a solution image (optional Pillow dependency).
- [bridson.py](bridson.py): Bridson Poisson disk sampling (2D) with optional plotting via matplotlib.
- [election.py](election.py): ranked-choice (instant-runoff) election; prints a winner and exports ballots to `election.csv`.
- [24bitmap_filter.c](24bitmap_filter.c): applies a Sobel edge-detection filter to 24-bit BMP images.
- [nwm.py](nwm.py): North-West Corner method for the transportation problem (operations research).
- [tp.c](tp.c): the same transportation-problem idea implemented in C.
- [cd.ml](cd.ml): small compression/decompression experiment over strings (byte-oriented).
- [compress_decompress.ml](compress_decompress.ml): bit-level compression experiments (includes CCITT-fax-style encoding).

Guide-like docs:

- [compile/README.md](compile/README.md): a walkthrough of preprocessing → compile → assemble → link.
- [daysOfC/README.md](daysOfC/README.md): C notes + small demos.

## Quick start

### Python

Run a script:

```bash
python3 <script>.py
```

Examples:

```bash
python3 election.py Alice Bob Charlie
python3 maze.py path/to/maze.txt
python3 bridson.py
python3 nwm.py
```

Optional dependencies (only for some scripts):

- [maze.py](maze.py): uses Pillow *only* if you call `output_image(...)` (`pip install pillow`)
- [bridson.py](bridson.py): uses matplotlib for plotting (`pip install matplotlib`)

### C

Compile a single C file:

```bash
gcc -O2 -Wall -Wextra -pedantic file.c -o prog
```

Notes:

- [24bitmap_filter.c](24bitmap_filter.c) uses `math.h` (Sobel operator), so you may need `-lm`:

```bash
gcc -O2 -Wall -Wextra -pedantic 24bitmap_filter.c -lm -o bmp_filter
```

### OCaml

Some OCaml files are written for the toplevel + `Graphics`.

```bash
ocaml
# or
utop
```
Then execute in the top level by typing `#use "julia_set.ml";;` - for example.
Notes:

- [mandelbrot_set.ml](mandelbrot_set.ml) and [julia_set.ml](julia_set.ml) reference a file named [complex.ml](complex.ml) via `#use "complex.ml";;` (that file is provided by the standard library).

## What’s inside (high level)

This repo isn’t a single cohesive project; it’s a collection of small exercises and experiments, for example:

- Algorithms / math utilities (e.g. determinants, matrix multiplication, postfix evaluation)
- Small CS exercises (e.g. change-making, credit-card checksum, ranked-choice runoff)
- Sampling / graphics-ish experiments (e.g. Poisson disk sampling, bitmap filtering)

If you’re looking for the more “guide-like” content, start with:

- [compile/README.md](compile/README.md)
- [daysOfC/README.md](daysOfC/README.md)
