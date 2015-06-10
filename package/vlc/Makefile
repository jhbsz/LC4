# $Id: Makefile 5624 2006-11-23 00:29:07Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=vlc
PKG_VERSION:=0.9.6
PKG_RELEASE:=1

PKG_SOURCE:=vlc-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://download.videolan.org/pub/videolan/vlc/$(PKG_VERSION)/$(PKG_SOURCE)
PKG_MD5SUM:=cd71276ed867029a6d077a40bccd4d05
PKG_CAT:=bzcat
 
PKG_BUILD_DIR:=$(BUILD_DIR)/vlc-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
 
define Package/vlc
   SECTION:=sound
   CATEGORY:=Sound
   TITLE:=Videolan Client
   DEPENDS:=+libflac +libid3tag +libmad +libvorbisidec +libffmpeg +libiconv +libvorbis
   URL:=http://www.videolan.org/
endef
 
define Package/vlc/description
   Videolan Client:
   a music player
endef
 
define Build/Configure
	$(call Build/Configure/Default, \
	 --with-linux-headers="$(LINUX_DIR)" \
	 --enable-run-as-root \
	 --disable-a52 \
	 --disable-dbus \
	 --disable-dbus-control \
	 --disable-vlm \
	 --disable-notify \
	 --disable-audioscrobbler \
	 --disable-live555 \
	 --disable-dvdnav \
	 --disable-dshow \
	 --disable-bda \
	 --disable-smb \
	 --disable-dvbpsi \
	 --disable-v4l2 \
	 --disable-libv4l2 \
	 --disable-libcdio \
	 --disable-libvcdinfo \
	 --disable-libcddb \
	 --disable-cdda \
	 --disable-vcd \
	 --disable-dvb \
	 --disable-screen \
	 --disable-mkv \
	 --disable-mod \
	 --disable-gme \
	 --disable-postproc \
	 --disable-quicktime \
	 --disable-libtar \
	 --disable-a52 \
	 --disable-libmpeg2 \
	 --disable-speex \
	 --disable-schroedinger \
	 --disable-png \
	 --disable-x264 \
	 --disable-fluidsynth \
	 --disable-cmml \
	 --disable-kate \
	 --disable-x11 \
	 --disable-xvideo \
	 --disable-glx \
	 --disable-xinerama \
	 --disable-opengl \
	 --disable-sdl \
	 --disable-sdl-image \
	 --disable-freetype \
	 --disable-fribidi \
	 --disable-fontconfig \
	 --disable-libxml2 \
	 --disable-hd1000v \
	 --disable-directx \
	 --disable-fb \
	 --disable-wingdi \
	 --disable-pulse \
	 --disable-waveout \
	 --disable-macosx-audio \
	 --disable-hd1000a \
	 --disable-upnp \
	 --disable-skins2 \
	 --disable-qt4 \
	 --disable-wince \
	 --disable-macosx \
	 --disable-visual \
	 --disable-bonjour \
	 --disable-libgcrypt \
	 --disable-gnutls \
	 --disable-remoteosd \
	 --disable-activex \
	 --disable-nls \
	 --disable-rpath \
	 --disable-telepathy \
	 --disable-cddax \
	 --disable-swscale \
	 --disable-dca \
	 --disable-telx \
	 --disable-zvbi \
	 --disable-atmo \
	 --disable-mozilla \
	 --enable-optimize-memory \
	 --with-libiconv-prefix=$(STAGING_DIR)/usr/lib/libiconv \
	 CFLAGS="$(CFLAGS) $(TARGET_CFLAGS) -std=gnu99" \
	)
endef

define Package/vlc/install
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR="$(1)" install
	rm -rf "${1}"/usr/include
	rm -f  "${1}"/usr/lib/vlc/*/*.la
	rm -f  "${1}"/usr/lib/vlc/*mux/lib*mp4*
	rm -f  "${1}"/usr/lib/vlc/video*/*
	rm -rf "${1}"/usr/share/applications
	rm -rf "${1}"/usr/share/doc
endef
 
$(eval $(call BuildPackage,vlc))
