module 'ctable' {
   lib {
      pch_src 'src/pch.cpp',
      define 'BE_CTABLE_IMPL'
   },
   app '-test' {
      icon 'icon/bengine-test-perf.ico',
      link_project {
         'testing',
         'core',
         'core-id'
      }
   }
}
