#use "complex.ml";;
#use "topfind";;
#require "graphics";;
open Graphics;;
open_graph "";;
resize_window 1001 1001;;
set_window_title "Julia Set";;

let shades_of_grey x =
  let colour = rgb x x x in
  set_color colour

let delta = 0.001
let number = 128
let x = ref {re = -0.501; im = -0.501}
let xre = ref (-0.501);;
let xim = ref (-0.501);;
let count = ref 0;;
let z = {re = -0.74543 ; im = 0.11301};;
for i = 0 to size_x () - 1 do 
  xre := !xre +. delta;
  xim := -0.501;
  for j = 0 to size_y () - 1 do 
    xim := !xim +. delta;
    count := 0;
    x := {re = !xre; im = !xim};
    while (norm !x <= 2. && !count < number) do 
      x := add (mul !x !x) z;
      count := !count + 1
    done;
    if norm !x <= 2. 
      then shades_of_grey 0
    else shades_of_grey (1 + truncate (254. *. float !count /. float number));
    plot i j
  done;
done;;

