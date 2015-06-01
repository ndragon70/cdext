# cdext spec file, used to drive rpmbuild

Summary:   This package provides extension commands to the bash cd command.
Name: 	   cdext
Version:   %{ver}
Release:   %{rel}
License:   BSD 
Group:     Software
URL:       http://www.newells.info
Source0:   http://prdownloads.sourceforge.net/cdext/%{name}-%{version}.tar.gz
Vendor:    James Newell <jnewell@newells.info>
Packager:  James Newell <jnewell@newells.info>
BuildRoot: %{_tmppath}/%{name}-%(id -un)

# include dependencies manually so the it installs most rpm distros
AutoReq:   0  
Requires:  bash

%description
This package provides extension commands to the bash cd command.

%prep
%setup -q

%build
%__make

%install
[ %{buildroot} != "/" ] && rm -rf %{buildroot}

# install man page
%__install -c -d -m 755 %{buildroot}%{_mandir}/man1
%__install -c -m 755 %{name}.1 %{buildroot}%{_mandir}/man1

# install binaries
%__install -c -d -m 755 %{buildroot}/usr/local/bin
%__install -c -m 755 %{name}-ccd %{buildroot}/usr/local/bin
%__install -c -m 755 %{name}-list %{buildroot}/usr/local/bin
%__install -c -m 755 %{name}-save %{buildroot}/usr/local/bin
%__install -c -m 755 %{name}-delete %{buildroot}/usr/local/bin
%__install -c -m 755 %{name}-setcd %{buildroot}/usr/local/bin

# install bash setup script
%__install -c -d -m 755 %{buildroot}/etc/%{name}
%__install -c -m 755 %{name}-setup.bash %{buildroot}/etc/%{name}/bash.conf
%__install -c -m 755 %{name}-setup.sh   %{buildroot}/etc/%{name}/sh.conf
%__install -c -m 755 %{name}-setup.ksh  %{buildroot}/etc/%{name}/ksh.conf
%__install -c -m 755 %{name}-setup.csh  %{buildroot}/etc/%{name}/csh.conf

%if "%{_vendor}" == "redhat"
%__install -c -d -m 755 %{buildroot}/etc/profile.d
%__install -c -m 755 %{name}-setup.bash %{buildroot}/etc/profile.d/%{name}.sh 
%endif

%clean
[ %{buildroot} != "/" ] && rm -rf %{buildroot}

%post

%postun

%files
%defattr(-,root,root)
/usr/local/bin/%{name}-ccd
/usr/local/bin/%{name}-delete
/usr/local/bin/%{name}-list
/usr/local/bin/%{name}-save
/usr/local/bin/%{name}-setcd
%{_mandir}/man1/%{name}.1*
/etc/%{name}/*

%if "%{_vendor}" == "redhat"
/etc/profile.d/%{name}.sh
%endif

%changelog 

* Fri Jun 17 2005 James Newell <jnewell@newells.info>
- Add support for all shells, and specific distros

* Thu Jun 16 2005 James Newell <jnewell@newells.info>
- Intial build.
