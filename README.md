    # Sketch core

# Download & Build

First download the program from GitHub and go to the sketch-core folder.

```
% git clone https://github.com/Sketch-File-Manager/sketch-core.git
% cd core/
```

After installation the program must be built. In order to build the program, the following instructions must be
followed.<br>

```
% mkdir build
% cd build/
% cmake ../
% make
% sudo make install
```

The cmake should be from version 3.20 and above, if you do not have this version please go to the following site and download the latest version:
https://cmake.org/download/

After this the program will be installed and ready to run.

## Commands

### mkdir
`core mkdir name`: Current path \
`core mkdir name permissions`: Current path
`core mkdir dst name`\
`core mkdir dst name permissions`

### mkfile
`core mkfile name: Current path` \
`core mkfile name permissions`: Current path \
`core mkfile dst name`\
`core mkfile dst name permissions`

### copy
`core copy dst src`

### move
`core move dst src`

### rename
`core rename src new_name`

### permissions
`core permissions src new_perms`\
`core permissions src new_perms --depth=depth`

### list
`core list`: Current path \
`core list src`

### delete
`core delete src`

## Session Commands

### mkdir
`core session-mkdir dst name`\
`core session-mkdir dst name permissions`

### mkfile
`core session-mkfile dst name`\
`core session-mkfile dst name permissions`

### copy
`core session-copy dst src`

### move
`core session-move dst src`

### rename
`core session-rename src new_name`

### permissions
`core session-permissions src new_perms`\
`core session-permissions src new_perms --depth=depth`

### list
`core session-list`\
`core session-list src`

### delete
`core session-delete src`

## Options

### Session file
`--session=path/to/session_file`

### Byte rate
`--byte-rate=516KB`