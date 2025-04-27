#use "vbrt.ml";;

let create_tuple min_x max_x min_y max_y =
    let rec range start stop a = 
        if start > stop then a
        else range (start + 1) stop (start::a)
    in 
    let list_x = range min_x max_x [] in
    let list_y = range min_y max_y [] in 
    let a l x = List.map  (fun y -> (x, y))  l
    in
    List.flatten (
        List.map  (a list_y)  list_x
        )

let shader o tmin tmax ls ll (x, y) = 
    let v = g_to_viewport x y in
            let d = sub3 v o in
            let color = rtx o d tmin tmax ls ll in 
            plotc x y color

let create_sphere radius center color shininess reflectiveness =
    { r = radius; c = center; color = color; s = shininess; rfl = reflectiveness }

(* Define a new function to create a light source *)
let create_light kind intensity position =
    { k = kind; i = intensity; v = position }
let main () =
    let o = {x = 0.;y = 0.;z = 0.} in (* the origin O*)
    
    let s1 = 
    {
        c = {x = 0.;y= - 1.; z = 3.};
        r = 1.;
        color = (255, 0, 0);
        s = 9900;
        rfl = 0.2
    } in 
    let s2 =
    {
        c = {x = -2.; y = 1.; z = 3.};
        r = 1.;
        color = (0, 0, 255);
        s = 5;
        rfl = 0.
    } in 
    let s3 =
    {
        c = {x =  2. ; y = 1. ; z = 3.};
        r = 1.;
        color = (0, 255, 0);
        s = 50;
        rfl = 0.5
    } in
    let s4 =
    {
        c = {x = 0.; y = - 5001.; z = 0.};
        r = 5000.;
        color = (255, 255, 0);
        s = 1000;
        rfl = 0.01
    } in
    (* list of sphere *)
    let ls = [s1;s2;s3;s4] in 
    let l1 =
    {
        k = 'a';
        i = 0.2;
        v = None
    } in
    let l2 =
    {
        k= 'p';
        i = 0.6;
        v = Some {x =2.;y= 1.;z= 0.}
    } in 
    let l3 =
    {
        k = 'd';
        i = 0.2;
        v = Some {x=1.;y= 4.;z= 4.}
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


