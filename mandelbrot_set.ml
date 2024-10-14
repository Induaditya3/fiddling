#use "complex.ml";;
#use "topfind";;
#require "graphics";;
open Graphics;;

open_graph "";;
set_window_title "Mandelbrot Set";;
resize_window 1001 1001;;
let shades_of_grey x =
  let colour = rgb x x x in
  set_color colour
let delta = 0.01
let number = 64
let zreal = ref (-5.01);;
let zimag = ref (-5.01);;
let count = ref 0;;
let x = ref zero;;
let z = ref zero;;
for i = 0 to size_x () -1 do
  zreal := !zreal +. delta;
  zimag := (-5.01);
  for j = 0 to size_y () -1 do
    begin
      zimag := !zimag +. delta ;
      count := 0;
      x := zero;
      z := {re = !zreal; im = !zimag};
      (* Printf.printf "x = %f + %fi \n" (!z).re (!z).im; *)
      while (norm !x <= 2. && !count < number) do
        (x := add (mul (!x) (!x)) !z;
        (* Printf.printf "x = %f + %fi \t count = %i \t z = %f + %fi\n" (!x).re (!x).im !count (!z).re (!z).im; *)
        count := !count + 1)
      done;
      (if (norm !x <= 2. ) then shades_of_grey 0
      else (let c = ((truncate(254. *. (float_of_int !count) /. (float_of_int number))) + 1) in
      shades_of_grey c));
      plot i j;
    end
  done;
done;;