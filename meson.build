project('scenegraph-demo', 'cpp')
add_global_arguments('-std=c++14', language : 'cpp')

incdir = include_directories('src')

sources = [
  'src/logging.cpp',
  'src/main.cpp',
  'src/nodes/node.cpp',
  'src/nodes/perspective_camera.cpp',
  'src/resources/image_resource.cpp',
  'src/resources/raw_resource.cpp',
  'src/resources/resource.cpp',
  'src/shaders/shader.cpp',
]

dependencies = [
  dependency('glew'),
  dependency('sdl2'),
  dependency('boost', modules : ['system', 'filesystem']),
]

if host_machine.system() == 'darwin'
  dependencies += dependency('appleframeworks', modules : ['OpenGL'], required : false)
endif

executable(
  'scenegraph-demo',
  sources : sources,
  dependencies : dependencies,
  include_directories : incdir,
  link_args : '-lSDL2_image'
)
