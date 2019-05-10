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
  - id: num_nodes
    type: u4
  - id: num_blub
    type: u4
  - id: num_vs2
    type: u4
  - id: num_names_w
    type: u4
  - id: num_something
    type: u4
  - id: num_names
    type: u4
  - id: num_meshes
    type: u4
  - id: num_geenidee
    type: u4
  - id: nodes 
    type: node
    repeat: expr
    repeat-expr: num_nodes
  - id: blub
    type: u2
    repeat: expr
    repeat-expr: num_blub * 2
  - id: vs2
    type: node
    repeat: expr
    repeat-expr: num_vs2
  - id: names_v3
    type: vector3
  - id: names_x
    type: u4
  - id: names_w
    type: u2
    repeat: expr
    repeat-expr: 2 * num_names_w
  - id: names
    size: 24
    type: strz
    encoding: ASCII
    repeat: expr
    repeat-expr: num_names
  - id: geenidee
    size: 28
    repeat: expr
    repeat-expr: num_geenidee
  - id: mesh
    type: mesh
    repeat: expr
    repeat-expr: num_meshes
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
        encoding: ASCII
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
        type: packed_vertex
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
  materials:
    seq:
      - id: hdr
        type: pershdr
      - id: version
        type: u4
      - id: skip
        type: u4
      - id: num_materials
        type: u4
      - id: materials
        type: material
        repeat: expr
        repeat-expr: num_materials
    types:
      material:
        seq:
          - id: texture_mode
            type: u1
            #enum: texture_mode
          - id: render_type
            type: u1
            #enum: render_type
          - id: padding
            size: 2
          - id: alpha
            type: f4
          - id: index
            type: u4
          - id: rgb
            type: packed_4b
          - id: name
            type: strz
            size: 32
            encoding: ASCII
  packed_vertex:
    seq:
     - id: z
       type: u1
     - id: x
       type: u1
     - id: y
       type: u1
     - id: scale
       type: u1
  packed_4b:
    seq:
      - id: r
        type: u1
      - id: g
        type: u1
      - id: b
        type: u1
      - id: a
        type: u1
enums:
  texture_mode:
    0: black
    1: rgb
    2: textured
  render_type:
    1: static_lighting
    2: face_lighting
    4: colored_lighting
    11: translucent11
    12: translucent12
    14: translucent14
    21: translucent21
    22: translucent22
    24: translucent24

