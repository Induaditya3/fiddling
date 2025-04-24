#use "topfind";;
#require "graphics";;
open Graphics;;

open_graph "";;
resize_window 768 768;;
set_window_title "ray tracer";;

type point = 
  {
    x : float;
    y : float;
    z : float
  };;

(* type for sphere *)
(* r - radius *)
(* c - center *)
(* color - color of the sphere *)
(* s - shininess of the sphere ranges 0 on up, -ve mean not shiny*)
(* rfl - reflectiveness of the sphere ranges from 0 to 1 *)
type sphere = 
  {
    r : float;
    c : point;
    color : int * int * int;
    s : int;
    rfl : float
  };;

(* type for light *)
(* kind of light is represented by k field - *)
(* p - for point source *)
(* d - for directional light *)
(* a - for ambient source *)
type light = 
  {
    k : char;
    i : float;
    v : point option
  }

let bare (Some p) = p;;

(* vector subraction *)
let sub3 p1 p2 = 
  {x = p1.x -. p2.x; y = p1.y -. p2.y; z = p1.z -. p2.z};;

(* vector addition *)
let add3 p1 p2 =
  {x = p1.x +. p2.x; y = p1.y +. p2.y; z = p1.z +. p2.z};;

(* scaling a vector *)
let scale k {x;y;z} =
  {x= k *. x;y = k *. y;z = k *. z};;

(* scalar product *)
let sproduct p1 p2 =
  p1.x *. p2.x +. p1.y *. p2.y +. p1.z *. p2.z;;

(* norm of a vector *)
let norm v = 
  sqrt (sproduct v v);;

let p1 = {x=1. ;y= 2. ;z=3. };;

let p2 = {x=2. ;y= 4. ;z=6. };;

(* solution of quadratic equation *)
let quad a b c = 
  let d = b *. b -. 4. *. a *. c in 
  if d >= 0. then 
    Some ((-. b +. sqrt d) /. (2. *. a)), Some ((-. b -. sqrt d) /. (2. *. a))
  else
    None, None;;

(* clamping function *)
let clamp x =
  if x > 255. then 255
  else if x < 0. then 0
  else truncate x;;

(* increasing brightness of a color and returning graphics' color  *)
let myrgb (x,y,z) intensity =
  let xi = float x *. intensity in 
  let yi = float y *. intensity in 
  let zi = float z *. intensity in 
  rgb (clamp xi) (clamp yi) (clamp zi);;

(* return sphere's color or background color *)
let sphere_color (sphere, intensity) =
  match sphere with 
  Some s -> myrgb s.color intensity
  | None -> rgb 255 255 255;; (*Background color is white*)

(* setting pixel color  *)
let plotc x' y' color =
  let x,y =  x' + (size_x ()) / 2, y' + (size_y ()) / 2 in 
  set_color color;
  plot x y;;

(* transforming coordinate from graphics window to scene *)
(* setting width vw and height vh of viewport *)
(* z coordinate of viewport is d *)
let g_to_viewport gx gy =
  let vwidth = 1. in 
  let vheight = 1. in
  let vx = (float gx *. vwidth)  /. float (size_x ()) in 
  let vy = (float gy *. vheight)  /. float (size_y ()) in 
  let d = 0.58 in 
  {x = vx;y = vy; z = d};;

(* finding point of intersection of ray with sphere and returning parameter t *)
let intersect_sphere o d s =
  let co = sub3 o s.c in 
  let a = sproduct d d in 
  let b = 2. *. sproduct d co in 
  let c = sproduct co co -. (s.r *. s.r) in 
  quad a b c;;

(* finding closest_sphere to the camera and parameter t of intersection with sphere with ray from camera *)
(* closest_s should initially be None *)
(* closest_t should intially be (Some infinity) *)
let rec closest_sphere_inner o d tmin tmax sl closest_s closest_t =
  let c_sphere = ref closest_s in
  let c_t = ref closest_t in
  match sl with 
  s1::sr ->
    let t1, t2 = intersect_sphere o d s1 in 
    if (t1 >= Some tmin && t1 <= Some tmax) && t1 <= !c_t then
      begin
        c_t := t1;
        c_sphere := Some s1
      end;
    if (t2 >= Some tmin && t2 <= Some tmax) && t2 <= !c_t then
      begin
        c_t := t2;
        c_sphere := Some s1
      end;
    closest_sphere_inner o d tmin tmax sr !c_sphere !c_t
  | _ -> closest_s, closest_t;;

let closest_sphere o d tmin tmax sl = closest_sphere_inner o d tmin tmax sl None (Some infinity);;

(* reflected ray *)
let reflected_ray ncap l =
  sub3 (scale (2. *. sproduct l ncap) ncap) l;;

(* diffuse reflection *)
let diffuse_i normal l i c_intensity =
  let nl = sproduct normal l in 
  let di = ref 0. in
  (if nl > 0. then
    di := c_intensity +. i *. nl /. (norm l *. norm normal)else di := c_intensity);
  !di;;

(* specular reflection *)
let specular_i o p normal l i s c_intensity =
  let reflected = reflected_ray normal l in
  let view = sub3 o p in 
  let rv = sproduct reflected view in 
  let si = ref 0. in
  (if rv > 0. && s > 0 then 
    si := c_intensity +. i *. (rv /. (norm reflected *. norm view))** (float s)
    else si := c_intensity);
    !si;;

(* total light intensity after reflection *)
let rec til_inner normal p o s light_l sphere_l intensity = 
  match light_l with 
  {k;i;v}::t -> 
    let c_intensity = ref 0. in
    let tmax = ref 0. in 
    if k = 'a' then c_intensity := i +. !c_intensity 
    else 
      begin
        let l = ref {x=0.;y=0.;z=0.} in
        (if k = 'p' then
            (l := (sub3 (bare v) p); 
            tmax := 1.)
          else  
           (l := bare v;
           tmax := infinity));
          let shadow_s,_ = closest_sphere p !l 0.0001 !tmax sphere_l in 
          match shadow_s with 
          Some ss -> ()
          |_ -> 
            let unitnormal = scale (1. /.(norm normal)) normal in 
          c_intensity := specular_i o p unitnormal !l i s !c_intensity;
          c_intensity := diffuse_i normal !l i !c_intensity
      end;
    til_inner normal p o s t sphere_l (intensity +. !c_intensity)
  | _ -> intensity;;

let til normal p o s light_l sphere_l = til_inner normal p o s light_l sphere_l 0.;;

let extract (s,i) =
 match s with 
 Some sphere -> i
 | _ -> 0.;;
let rec rtx_inner o d tmin tmax sl ll limit =
  let objintensity = ref 0. in 
  let otherintensity = ref 0. in  
  let s, t = closest_sphere o d tmin tmax sl in 
  begin
    match s, t with 
    Some sphere, Some t_parameter ->
      let p = add3 o (scale t_parameter d) in 
      let normal = sub3 p sphere.c in
      objintensity := til normal p o sphere.s ll sl;
      if sphere.rfl > 0. && limit > 0 then
        begin
          let reflected = reflected_ray (scale (1. /. norm normal) normal) (scale (- 1.) d) in 
          otherintensity := extract (rtx_inner p reflected 0.001 infinity sl ll (limit-1));
          objintensity := !objintensity *. (1. -. sphere.rfl) +. sphere.rfl *. !otherintensity;
        end
    | _, _ -> ()
  end; 
  s, !objintensity;;

let rtx o d tmin tmax sl ll = sphere_color (rtx_inner o d tmin tmax sl ll 3)