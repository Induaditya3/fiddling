type point = { x : float; y : float; z : float; }
type sphere = {
  r : float;
  c : point;
  color : int * int * int;
  s : int;
  rfl : float;
}
type light = { k : char; i : float; v : point option; }
val bare : 'a option -> 'a
val sub3 : point -> point -> point
val add3 : point -> point -> point
val scale : float -> point -> point
val sproduct : point -> point -> float
val norm : point -> float
val p1 : point
val p2 : point
val quad : float -> float -> float -> float option * float option
val clamp : float -> int
val myrgb : int * int * int -> float -> 'a
val sphere_color : sphere option * float -> 'a
val plotc : int -> int -> 'a -> 'b
val g_to_viewport : int -> int -> point
val intersect_sphere :
  point -> point -> sphere -> float option * float option
val closest_sphere_inner :
  point ->
  point ->
  float ->
  float ->
  sphere list ->
  sphere option -> float option -> sphere option * float option
val closest_sphere :
  point ->
  point -> float -> float -> sphere list -> sphere option * float option
val reflected_ray : point -> point -> point
val diffuse_i : point -> point -> float -> float -> float
val specular_i :
  point -> point -> point -> point -> float -> int -> float -> float
val til_inner :
  point ->
  point -> point -> int -> light list -> sphere list -> float -> float
val til :
  point -> point -> point -> int -> light list -> sphere list -> float
val extract : 'a option * float -> float
val rtx_inner :
  point ->
  point ->
  float -> float -> sphere list -> light list -> int -> sphere option * float
val rtx : point -> point -> float -> float -> sphere list -> light list -> 'a
