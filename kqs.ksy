meta:
  id: kqs
  file-extension: kqs
  endian: le
seq:
  - id: hdr
    type: pershdr
  - type: u4
    id: version
  - id: radius
    type: f4
  - id: center
    type: vector3
  - id: lens
    type: u4
    repeat: expr
    repeat-expr: 8
  - id: nodes 
    type: node
    repeat: expr
    repeat-expr: lens[0]
  - id: blub
    type: u2
    repeat: expr
    repeat-expr: lens[1] * 2
  - id: vs2
    type: node
    repeat: expr
    repeat-expr: lens[2]
  - id: namesx
    type: names
  - id: names
    size: 24
    type: strz
    encoding: UTF-8
    repeat: expr
    repeat-expr: lens[6]
  - id: mesh
    type: mesh
    repeat: expr
    repeat-expr: lens[6]
  - id: has_materials
    type: u4
  - id: materials
    type: materials
    if: has_materials & 1 == 1
types:
  pershdr:
    seq:
      - id: magic
        contents: PERS
      - type: u4
        id: chunk_size
      - id: tag_len
        type: u2
      - id: tag
        type: str
        size: tag_len + (tag_len & 1)
        encoding: UTF-8
  mesh:
    seq:
      - id: hdr
        type: pershdr
      - type: u4
        id: version
      - id: num_verts
        type: u4
      - id: verts_per_frame
        type: u4
      - id: num_tex_verts
        type: u4
      - id: num_faces
        type: u4
      - id: num_frames
        type: u4
      - id: tex_verts_per_frame
        type: u4
      - id: radius
        type: f4
      - id: packed_vertices
        type: u4
        repeat: expr
        repeat-expr: num_verts
      - id: tex_verts
        type: tex_vert
        repeat: expr
        repeat-expr: num_tex_verts
      - id: faces
        type: face
        repeat: expr
        repeat-expr: num_faces
      - id: frames
        type: frame
        repeat: expr
        repeat-expr: num_frames
    types:
      tex_vert:
        seq:
          - id: x
            type: f4
          - id: y
            type: f4
      face:
        seq:
          - id: v0
            type: s4
          - id: vt0
            type: s4
          - id: v1
            type: s4
          - id: vt1
            type: s4
          - id: v2
            type: s4
          - id: vt2
            type: s4
          - id: mat
            type: s4
      frame:
        seq:
          - id: first
            type: s4
          - id: scale
            type: vector3
          - id: origin
            type: vector3
  vector3:
    seq:
      - id: xyz
        type: f4
        repeat: expr
        repeat-expr: 3
  node:
    seq:
      - id: hdr
        type: u4
      - id: rotation
        type: vector3
        repeat: expr
        repeat-expr: 3
      - id: translation
        type: vector3
      - id: crap
        type: u2
        if: hdr & 4 == 4
      - id: name
        type: u2
      - id: skip0
        type: u2
        if: hdr & 4 == 4
      - id: skip
        type: u2
  names:
    seq:
      - id: v3
        type: vector3
      - id: x
        type: u4
      - id: y
        type: u2
      - id: z
        type: u2
      - id: animation_name
        type: strz
        size: 24
        encoding: UTF-8
  materials:
    seq:
      - id: hdr
        type: pershdr
      - id: version
        type: u4
      - type: u4
        id: num_materials
      - id: materials
        type: material
        repeat: expr
        repeat-expr: num_materials
    types:
      material:
        seq:
          - id: flags
            type: u4
          - id: alpha
            type: f4
          - id: index
            type: u4
          - id: junk
            type: u4
          - id: rgb_maybe
            type: u4
          - id: name
            type: strz
            size: 32
            encoding: UTF-8
