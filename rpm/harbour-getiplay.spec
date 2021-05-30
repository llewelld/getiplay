Name:       harbour-getiplay

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    User interface for get_iplayer
Version:    0.9
Release:    1
License:    LICENSE
URL:        http://www.flypig.co.uk/?to=getiplay
Source0:    %{name}-%{version}.tar.bz2
AutoReq:    0
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   perl >= 5.8.8
Requires:   qt5-qtdeclarative-import-xmllistmodel
Requires:   perl-Digest-SHA
Requires:   ffmpeg-tools
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  curl
BuildRequires:  perl-devel
BuildRequires:  openssl-devel
BuildRequires:  openssl
BuildRequires:  zlib-devel
AutoProv:   0
Patch1: 0001-Reduce-cmake-minimum-version-requirement.patch

%description
Short description of my SailfishOS Application

%prep
%autosetup -N -n %{name}-%{version}
cd atomicparsley
%patch1 -p1
cd ..

%build
%qtc_qmake5 
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install
desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}/lib
%{_datadir}/%{name}/qml
%{_datadir}/%{name}/translations
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%attr(755,root,root) %{_datadir}/%{name}/bin/get_iplayer
%attr(755,root,root) %{_datadir}/%{name}/bin/AtomicParsley
