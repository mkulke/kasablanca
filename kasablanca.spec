Summary: A Kde Ftp Client
Name: kasablanca
Version: 0.4.0.2
Release: 1
Copyright: GPL
Group: Applications/Internet
Source: http://download.berlios.de/kasablanca/kasablanca-0.4.0.2.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
Provides: kasablanca

%description
Kasablanca is a KDE ftp client.

Its main features are:

- advanced ftp operations like resumed and recursive transfers
- direct remote to remote transfer (fxp)
- ftps encryption on both control and data connection
- enqeueing of files and directories
- high configurability
%prep
%setup

%build
./configure --prefix=/var/tmp/%{name}-buildroot/opt/kde3
make

%install
make install

%clean
rm -rf /var/tmp/%{name}-buildroot

%files
/opt/kde3/bin/kasablanca
/opt/kde3/share/applnk/Utilities/kasablanca.desktop
/opt/kde3/share/apps/kasablanca/about.png
/opt/kde3/share/apps/kasablanca/kasablancaui.rc
/opt/kde3/share/config.kcfg/kbconfig.kcfg
/opt/kde3/share/doc/HTML/en/kasablanca/index.cache.bz2
/opt/kde3/share/doc/HTML/en/kasablanca/index.docbook
/opt/kde3/share/icons/hicolor/128x128/apps/kasablanca.png
/opt/kde3/share/icons/hicolor/16x16/apps/kasablanca.png
/opt/kde3/share/icons/hicolor/22x22/apps/kasablanca.png
/opt/kde3/share/icons/hicolor/32x32/apps/kasablanca.png
/opt/kde3/share/icons/hicolor/48x48/apps/kasablanca.png
/opt/kde3/share/icons/hicolor/64x64/apps/kasablanca.png
/opt/kde3/share/locale/de/LC_MESSAGES/kasablanca.mo
/opt/kde3/share/locale/pl/LC_MESSAGES/kasablanca.mo
