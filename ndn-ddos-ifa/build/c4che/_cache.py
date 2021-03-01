AR = '/usr/bin/ar'
ARFLAGS = 'rcs'
BINDIR = '/usr/local/bin'
BOOST_VERSION = '1_65_1'
CC_VERSION = ('7', '5', '0')
CFLAGS_NS3_APPLICATIONS = ['-pthread']
CFLAGS_NS3_CORE = ['-pthread']
CFLAGS_NS3_INTERNET = ['-pthread']
CFLAGS_NS3_MOBILITY = ['-pthread']
CFLAGS_NS3_NDNSIM = ['-pthread']
CFLAGS_NS3_NETWORK = ['-pthread']
CFLAGS_NS3_POINT-TO-POINT = ['-pthread']
CFLAGS_NS3_TOPOLOGY-READ = ['-pthread']
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXX11_CMD = '-std=c++11'
CXXFLAGS = ['-O3', '-g']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_NS3_APPLICATIONS = ['-pthread']
CXXFLAGS_NS3_CORE = ['-pthread']
CXXFLAGS_NS3_INTERNET = ['-pthread']
CXXFLAGS_NS3_MOBILITY = ['-pthread']
CXXFLAGS_NS3_NDNSIM = ['-pthread']
CXXFLAGS_NS3_NETWORK = ['-pthread']
CXXFLAGS_NS3_POINT-TO-POINT = ['-pthread']
CXXFLAGS_NS3_TOPOLOGY-READ = ['-pthread']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'gcc'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DEFINES = ['HAVE_NS3_NDNSIM=1', 'HAVE_NS3_CORE=1', 'HAVE_NS3_NETWORK=1', 'HAVE_NS3_INTERNET=1', 'HAVE_NS3_POINT_TO_POINT=1', 'HAVE_NS3_TOPOLOGY_READ=1', 'HAVE_NS3_APPLICATIONS=1', 'HAVE_NS3_MOBILITY=1', 'NS3_LOG_ENABLE=1', 'NS3_ASSERT_ENABLE=1']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'x86_64'
DEST_OS = 'linux'
INCLUDES_BOOST = '/usr/include'
INCLUDES_NS3_APPLICATIONS = ['/usr/local/include/ns3-dev', '/usr/include/gtk-2.0', '/usr/lib/x86_64-linux-gnu/gtk-2.0/include', '/usr/include/gio-unix-2.0/', '/usr/include/cairo', '/usr/include/pango-1.0', '/usr/include/atk-1.0', '/usr/include/pixman-1', '/usr/include/gdk-pixbuf-2.0', '/usr/include/libpng16', '/usr/include/harfbuzz', '/usr/include/glib-2.0', '/usr/lib/x86_64-linux-gnu/glib-2.0/include', '/usr/include/freetype2', '/usr/include/libxml2']
INCLUDES_NS3_CORE = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_INTERNET = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_MOBILITY = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_NDNSIM = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_NETWORK = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_POINT-TO-POINT = ['/usr/local/include/ns3-dev']
INCLUDES_NS3_TOPOLOGY-READ = ['/usr/local/include/ns3-dev']
LIBDIR = '/usr/local/lib'
LIBPATH_BOOST = ['/usr/lib64']
LIBPATH_NS3_APPLICATIONS = ['/usr/local/lib']
LIBPATH_NS3_CORE = ['/usr/local/lib']
LIBPATH_NS3_INTERNET = ['/usr/local/lib']
LIBPATH_NS3_MOBILITY = ['/usr/local/lib']
LIBPATH_NS3_NDNSIM = ['/usr/local/lib']
LIBPATH_NS3_NETWORK = ['/usr/local/lib']
LIBPATH_NS3_POINT-TO-POINT = ['/usr/local/lib']
LIBPATH_NS3_TOPOLOGY-READ = ['/usr/local/lib']
LIBPATH_ST = '-L%s'
LIB_BOOST = ['boost_system', 'boost_iostreams']
LIB_NS3_APPLICATIONS = ['ns3-dev-applications-optimized', 'ns3-dev-internet-optimized', 'ns3-dev-bridge-optimized', 'ns3-dev-mpi-optimized', 'ns3-dev-config-store-optimized', 'gtk-x11-2.0', 'gdk-x11-2.0', 'pangocairo-1.0', 'atk-1.0', 'cairo', 'gdk_pixbuf-2.0', 'gio-2.0', 'pangoft2-1.0', 'pango-1.0', 'gobject-2.0', 'glib-2.0', 'fontconfig', 'freetype', 'xml2', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_CORE = ['ns3-dev-core-optimized', 'rt']
LIB_NS3_INTERNET = ['ns3-dev-internet-optimized', 'ns3-dev-bridge-optimized', 'ns3-dev-mpi-optimized', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_MOBILITY = ['ns3-dev-mobility-optimized', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_NDNSIM = ['ns3-dev-ndnSIM-optimized', 'ns3-dev-point-to-point-optimized', 'ns3-dev-internet-optimized', 'ns3-dev-bridge-optimized', 'ns3-dev-mpi-optimized', 'ns3-dev-topology-read-optimized', 'ns3-dev-mobility-optimized', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_NETWORK = ['ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_POINT-TO-POINT = ['ns3-dev-point-to-point-optimized', 'ns3-dev-mpi-optimized', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_NS3_TOPOLOGY-READ = ['ns3-dev-topology-read-optimized', 'ns3-dev-network-optimized', 'ns3-dev-stats-optimized', 'sqlite3', 'ns3-dev-core-optimized', 'rt']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_NS3_APPLICATIONS = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_CORE = ['-pthread', '-Wl,-Bdynamic']
LINKFLAGS_NS3_INTERNET = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_MOBILITY = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_NDNSIM = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_NETWORK = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_POINT-TO-POINT = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_NS3_TOPOLOGY-READ = ['-pthread', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic', '-Wl,-Bdynamic']
LINKFLAGS_cxxshlib = ['-shared']
LINKFLAGS_cxxstlib = ['-Wl,-Bstatic']
LINK_CXX = ['/usr/bin/g++']
NS3_CHECK_MODULE_ONCE = ''
NS3_MODULES_FOUND = ['ndnSIM', 'core', 'network', 'internet', 'point-to-point', 'topology-read', 'applications', 'mobility']
PKGCONFIG = '/usr/bin/pkg-config'
PREFIX = '/usr/local'
RPATH_ST = '-Wl,-rpath,%s'
SHLIB_MARKER = ['-Wl,-Bdynamic', '-Wl,--no-as-needed']
SONAME_ST = '-Wl,-h,%s'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = '-Wl,-Bstatic'
STLIB_ST = '-l%s'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.so'
cxxstlib_PATTERN = 'lib%s.a'
define_key = ['HAVE_NS3_NDNSIM', 'HAVE_NS3_CORE', 'HAVE_NS3_NETWORK', 'HAVE_NS3_INTERNET', 'HAVE_NS3_POINT_TO_POINT', 'HAVE_NS3_TOPOLOGY_READ', 'HAVE_NS3_APPLICATIONS', 'HAVE_NS3_MOBILITY', 'NS3_LOG_ENABLE', 'NS3_ASSERT_ENABLE']
macbundle_PATTERN = '%s.bundle'