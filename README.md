# ``FFTW-Age``: FFTW Wisdom Generation

> "_With age comes wisdom, but sometimes age comes alone._"
> &emsp;&mdash; Oscar Wilde

``FFTW-Age`` is a lightweight  C++ software package for generating FFTW wisdom
files for OpenMP-multithreaded 3-d FFT transforms as a replacement for
[`fftw-wisdom`]( https://www.fftw.org/fftw-wisdom.1.html).


## Installation

The C++ program can be set up with `make`, provided that dependency
requirements are satisfied (FFTW3 and OpenMP libraries).

> [!IMPORTANT]
> Ensure the C++ compiler used supports OpenMP and
> the FFTW3 library is installed/configured accordingly.
> The default [``Makefile``](Makefile) (located at the repository directory root)
> assumes the GCC compiler and OpenMP library.

First `git clone` the desired branch/release from the GitHub repository and
change into the repository directory path:

```console
$ git clone git@github.com:MikeSWang/FFTW-Age.git --branch <branch-or-release>
$ cd FFTW-Age
```

Then, execute in shell:

```console
$ make clean && make install
```


## Documentation

Once installed, the FFTW-Age program can be called from the command line:

```console
$ fftw-age [--rigour <tplan>] [--outdir <wisdom-dir>] <tdir>-<dimx>x<dimy>x<dimz>
```

where

- `fftw-age` is assumed to be in the current working directory,
  or else replace it with the correct path;
- ``<tdir>`` is the transform direction,
  either ``"f"`` for ``FFTW_FORWARD`` and ``"b"`` for ``FFTW_BACKWARD``;
- ``<dimx>``, ``<dimy>`` and ``<dimz>`` are the dimensions of the 3-d transform;
- ``<tplan>`` corresponds to the FFTW planner flag/rigour level,
  with ``"m"`` (measured; default) for ``FFTW_MEASURE``
  and ``"p"`` (patient) for ``FFTW_PATIENT``;
- ``<wisdom-dir>`` is the output wisdom file directory
  (absolute or relative to the current working directory).

The output wisdom file will have the filename
``fftw_omp_<ttype><ttarr><tdir>_<dimx>x<dimy>x<dimz>_<tplan>.wisdom``.

> [!IMPORTANT]
> Only complex-to-complex, in-place transforms are currently supported.  Hence
> ``<ttype>`` is fixed to ``"c"`` (complex-to-complex) and
> ``<ttarr>`` is fixed to ``"i"`` (in-place) for now.


## Attribution

To acknowledge the use of ``FFTW-Age`` in your published research, please
cite this repository; you can refer to the file [``CITATION.cff``](CITATION.cff)
for the relevant information.


## Acknowledgement

This C++ package makes use of the ``argparse`` library from
[``p-ranav/argparse``](https://github.com/p-ranav/argparse).


## Contributing/Development

[![ReleaseDate](https://img.shields.io/github/release-date-pre/MikeSWang/FFTW-Age)](https://github.com/MikeSWang/FFTW-Age/releases/latest)
![CommitsSince](https://img.shields.io/github/commits-since/MikeSWang/FFTW-Age/latest/main)

[![Build-Issues](https://img.shields.io/github/issues/MikeSWang/FFTW-Age/build)](https://github.com/MikeSWang/FFTW-Age/issues?q=is%3Aopen+is%3Aissue+label%3Abuild)
[![Bug-Issues](https://img.shields.io/github/issues/MikeSWang/FFTW-Age/bug)](https://github.com/MikeSWang/FFTW-Age/issues?q=is%3Aopen+is%3Aissue+label%3Abug)
[![Feature-Issues](https://img.shields.io/github/issues/MikeSWang/FFTW-Age/feature)](https://github.com/MikeSWang/FFTW-Age/issues?q=is%3Aopen+is%3Aissue+label%3Afeature)
[![PullRequests](https://img.shields.io/github/issues-pr/MikeSWang/FFTW-Age)](https://github.com/MikeSWang/FFTW-Age/pulls)

[![Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/MikeSWang/FFTW-Age?hide_repo_select=true&ref=main)

User feedback and contributions are very welcome. Please refer to the
[contribution guidelines](.github/CONTRIBUTING.md).


## Discussions & Wiki

[![Discussions](https://img.shields.io/github/discussions/MikeSWang/FFTW-Age)](https://github.com/MikeSWang/FFTW-Age/discussions)

A [community forum](https://github.com/MikeSWang/FFTW-Age/discussions)
for users and developers exists, where you can receive
announcements, post questions, share ideas and get updates.

A [wiki site](https://github.com/MikeSWang/FFTW-Age/wiki) collects wisdoms
for specific use cases and user environments.


## Releases

Release notes are included in the [change log](CHANGELOG.md).


## Licence

[![Licence](https://img.shields.io/github/license/MikeSWang/FFTW-Age?label=licence&style=flat-square&color=informational)](https://github.com/MikeSWang/FFTW-Age/blob/main/LICENCE)

``FFTW-Age`` is made freely available under the [GPL-3.0 licence](
https://www.gnu.org/licenses/gpl-3.0.en.html) (or any later version).
Please see [``LICENCE``](LICENCE) (located at the repository directory root)
for full terms and conditions.

&copy; 2024 Mike S Wang
