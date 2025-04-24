#use "vbrt.ml";;
#require "parmap";;

let create_tuple min_x max_x min_y max_y =
    let rec range start stop a = 
        if start > stop then a
        else range (start + 1) stop (start::a)
    in 
    let list_x = range min_x max_x [] in
    let list_y = range min_y max_y [] in 
    let a l x = Parmap.parmap ~ncores:4 (fun y -> (x, y)) (Parmap.L l) in
    List.flatten (
        Parmap.parmap ~ncores:4 (a list_y) (Parmap.L list_x)
        )

let shader o tmin tmax ls ll (x, y) = 
    let v = g_to_viewport x y in
            let d = sub3 v o in
            let color = rtx o d tmin tmax ls ll in 
            plotc x y color
let main () =
    let o = {x = 0.;y = 0.;z = 0.} in (* the origin O*)
    let s1 = 
    {
        c = {x = 0.; y = -1.; z = 5.};
        r = 1.;
        color = (255, 0, 0);  (* Red *)
        s = 1000;             (* High shininess *)
        rfl = 0.5             (* Reflective *)
    } in 
    let s2 =
    {
        c = {x = -2.; y = 0.; z = 6.};
        r = 1.;
        color = (0, 0, 255);  (* Blue *)
        s = 5;                (* Low shininess *)
        rfl = 0.0             (* Not reflective *)
    } in 
    let s3 =
    {
        c = {x = 2.; y = 0.; z = 6.};
        r = 1.;
        color = (0, 255, 0);  (* Green *)
        s = 50;               (* Medium shininess *)
        rfl = 0.3             (* Slightly reflective *)
    } in
    let s4 =
    {
        c = {x = 0.; y = -5001.; z = 0.};
        r = 5000.;
        color = (255, 255, 0);  (* Yellow ground *)
        s = 1000;               (* High shininess *)
        rfl = 0.1               (* Slightly reflective *)
    } in
    let s5 =
    {
        c = {x = -1.; y = 1.; z = 4.};
        r = 0.5;
        color = (255, 165, 0);  (* Orange *)
        s = 300;                (* High shininess *)
        rfl = 0.4               (* Reflective *)
    } in
    let s6 =
    {
        c = {x = 1.; y = 1.; z = 4.};
        r = 0.5;
        color = (128, 0, 128);  (* Purple *)
        s = 200;                (* Medium shininess *)
        rfl = 0.2               (* Slightly reflective *)
    } in

    (* List of spheres *)
    let ls = [s1; s2; s3; s4; s5; s6] in 

    (* Define light sources *)
    let l1 =
    {
        k = 'a';
        i = 0.3;  (* Ambient intensity *)
        v = None
    } in
    let l2 =
    {
        k = 'p';
        i = 0.8;  (* Point light intensity *)
        v = Some {x = 2.; y = 3.; z = 1.}  (* Position of point light *)
    } in 
    let l3 =
    {
        k = 'd';
        i = 0.5;  (* Directional light intensity *)
        v = Some {x = -1.; y = -1.; z = -1.}  (* Direction of light *)
    } in 
    let ll = [l1;l2;l3] in 
    let gw, gh = size_x (), size_y () in (*max width and height of graphics window *)
    (* let coordinates = create_tuple (-gw/2) (gw/2) (-gh/2) (gh/2) in 
    List.iter (shader o 1. infinity ls ll ) coordinates *)
    for y = - gh/2 to gh/2 do 
    for x = - gw/2 to gw/2 do 
        let v = g_to_viewport x y in
        let d = sub3 v o in
        let color = rtx o d 1. infinity ls ll in 
        plotc x y color
    done;
done

let () = main ()


