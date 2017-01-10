module 'ctable' {
   lib {
      pch_src 'src/pch.cpp',
      define 'BE_CTABLE_IMPL',
      link_project {
         'core', 'util'
      }
   },
   app '-test' {
      icon 'icon/bengine-test-perf.ico',
      link_project 'testing'
   }
}
