(* gui for cryptogrammer
 * built with lablgtk *)


(* create a window *)
let create_window () =
  let window = GWindow.window ~title:"Cryptogrammer" ~width:400 ~height:250 () in
  ignore (window#event#connect#delete ~callback:(fun ev -> false));
  ignore (window#connect#destroy ~callback:(fun () -> GMain.Main.quit ()));
  window

(* generate button action *)
let generate (src:GText.view) (dest:GText.view) () =
  let tentry = src#buffer#get_text ~start:src#buffer#start_iter ~stop:src#buffer#end_iter () in
  let result = Logic.crypto tentry in
  dest#buffer#set_text result


(* create a text area *)
let create_text_area edit cont init_string =
  let sw = GBin.scrolled_window ~border_width:10 ~hpolicy:`AUTOMATIC ~vpolicy:`AUTOMATIC ~packing:cont#add () in
  let v = GText.view ~packing:sw#add ~border_width:5 ~editable:edit () in
  v#buffer#set_text init_string;
  v
  


(* main program *)
let main () =
  Logic.init ();
  
  let window = create_window () in
  window#show ();

  (* make a vertical box for text and then buttons *)
  let vbox = GPack.box `VERTICAL ~packing:window#add () in

  (* create a scrolled-window for text *)
  let entry = create_text_area true vbox "Enter Text Here" in

  (* add an hbox for buttons *)
  let hbox = GPack.box `HORIZONTAL ~packing:vbox#pack () in

  (* add a Generate button *)
  let b1 = GButton.button ~label:"Generate" ~packing:(hbox#pack ~expand:true) () in
  
  (* add a Clear button *)
  let b2 = GButton.button ~label:"Clear" ~packing:(hbox#pack ~expand:true) () in

  (* add a text area for result *)
  let result = create_text_area false vbox "Result Will Appear Here" in

  (* connect button actions *)
  ignore (b1#connect#clicked ~callback:(generate entry result));
  
  let del_text () =
    let del_aux thing =
      thing#buffer#delete ~start:thing#buffer#start_iter ~stop:thing#buffer#end_iter
    in
    del_aux entry;
    del_aux result
  in
  ignore (b2#connect#clicked ~callback:del_text);

  (* begin main GTK loop *)
  GMain.Main.main ()
;;

(* run main *)
main ();



