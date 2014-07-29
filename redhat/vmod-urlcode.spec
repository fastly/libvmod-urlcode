Summary: urlencode/urldecode support for Varnish VCL
Name: vmod-urlcode
Version: 0.1
Release: 1%{?dist}
License: BSD
Group: System Environment/Daemons
Source0: libvmod-urlcode.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: varnish > 3.0
BuildRequires: make, autoconf, automake, libtool, python-docutils

%description
libvmod-urlcode

%prep
%setup -n libvmod-urlcode
./autogen.sh

%build
# this assumes that VARNISHSRC is defined on the rpmbuild command line, like this:
# rpmbuild -bb --define 'VARNISHSRC /home/user/rpmbuild/BUILD/varnish-3.0.3' redhat/*spec
./configure VARNISHSRC=%{VARNISHSRC} VMODDIR=/usr/lib64/varnish/vmods/ --prefix=/usr/  --docdir='${datarootdir}/doc/%{name}'

make

%install
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
/usr/lib64/varnish/vmods/

%doc

%if "%{RHVERSION}" == "EL5"
/usr/man/man?/*
%else
/usr/share/man/man?/*
%endif

