# FFTW-Age: FFTW Wisdom Generation

``FFTW-Age`` is a C++ software package for generating FFTW wisdom files.


## Documentation

Coming soon...


## Installation

The C++ program can be set up with `make`, provided that dependency
requirements are satisfied (FFTW3 and OpenMP libraries).

> [!IMPORTANT]
> Ensure the C++ compiler used supports OpenMP and is configured accordingly.
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


## Attribution

To acknowledge the use of ``FFTW-Age`` in your published research, please
cite this repository; you can refer to the file [``CITATION.cff``](CITATION.cff)
for the relevant information.


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
