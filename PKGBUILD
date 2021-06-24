pkgname=worldlinesim-git
pkgver=1
pkgrel=1
pkgdesc="A simple Multiverse simulator written in C++, based on WorldLine Theory (Stein;s Gate)"
arch=('x86_64')
url="https://github.com/adi-g15/worldLineSim"
license=('MIT')
depends=()
makedepends=('gcc' 'git' 'cmake' 'ninja')
provides=("${pkgname%-git}")
conflicts=("${pkgname%-git}")
source=('worldlinesim::git+https://github.com/adi-g15/worldLineSim')
md5sums=('SKIP')

pkgver() {
        cd "$srcdir/${pkgname%-git}"

	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

prepare() {
	cd "$srcdir/${pkgname%-git}"

	git submodule --update --depth=1
}

build() {
	cd "$srcdir/${pkgname%-git}"
        mkdir -p build && cd build
        rm -f CMakeCache.txt
	cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
	cmake --build .
}

package() {
	cd "$srcdir/${pkgname%-git}"
        cd build
	DESTDIR="$pkgdir/" cmake --install .
}
