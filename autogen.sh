#!/usr/bin/env sh

die() {
    echo "Aborting $(basename $0): $@"
    exit 1
}

libtoolize || die "libtoolize failed"
aclocal || die "aclocal failed"
automake --add-missing --force-missing --copy --foreign || die "automake failed"
autoreconf || die "autoreconf failed"
