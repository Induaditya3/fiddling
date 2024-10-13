
let string_of_int_list l =
let b = Bytes.create (List.length l) in
List.iteri (fun n x -> Bytes.set b n (char_of_int x)) l;
Bytes.to_string b;;


let int_list_of_string s =
let l = ref [] in
for x = String.length s -1 downto 0 do
  l := int_of_char s.[x] :: !l
done;
!l;;



type output =
{output_char : char -> unit;
out_channel_length : unit -> int};;


let output_of_buffer b =
{output_char = Buffer.add_char b ;
out_channel_length = (fun () -> Buffer.length b)};;

type input =
{pos_in : unit -> int;
seek_in : int -> unit;
input_char : unit -> char;
in_channel_length : int};;


let input_of_string s =
let pos = ref 0 in
{pos_in = (fun () -> !pos);
seek_in = (fun n -> 
  if n < 0 then
    raise (Invalid_argument "seek before beginning");
  pos := n);
input_char = (fun () -> if !pos < String.length s then
  let c = s.[!pos] in pos := !pos + 1; c
else raise End_of_file);
in_channel_length = String.length s};;


let process f s = 
let b = Buffer.create (String.length s) in 
f (input_of_string s) (output_of_buffer b);
Buffer.contents b;;

exception EOD;;


let rewind i = i.seek_in (i.pos_in () -1);;


let get_same i = 
let rec get_count ch n =
  if n = 128 then 128 else
  try 
  if ch = i.input_char () then get_count ch (n+1) 
  else (rewind i; n)
  with End_of_file -> n 
 in
let ch = i.input_char () in (ch, get_count ch 1);;

let get_different i =
let rec getdiffinner a n =
 if n = 128 then List.rev a else
try
 let ch' = i.input_char () in
 if ch' <> List.hd a 
   then getdiffinner (ch' :: a) (n+1)
 else (rewind i; rewind i; List.rev (List.tl a))
with End_of_file -> List.rev a 
 in
getdiffinner [i.input_char ()] 1;;

let compress i o =
try 
  while true do
  match get_same i with
  (_, 1) -> rewind i; let l = get_different i in
            o.output_char (char_of_int (List.length l -1));
            List.iter o.output_char l
|  (c, n) -> o.output_char (char_of_int (257-n));
            o.output_char c
  done;
with End_of_file -> o.output_char (char_of_int 128);;

let compress_string = process compress;;




let decompress i o =
try
  while true do
    match int_of_char (i.input_char ()) with
x when x >=0 && x < 128 ->
  for p = 1 to x+1 do o.output_char (i.input_char ()) done
|x when x > 128 && x < 256 -> let ch = i.input_char () in for p = 1 to 257-x do o.output_char ch done
| _ -> raise EOD
  done
with 
  EOD -> ();;

let decompress_string = process decompress;;
