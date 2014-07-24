(* logic.ml
 * cryptogrammer *)

let init () =
  Random.self_init ()

let shuffled_letters () =
  let letters = [|'A'; 'B'; 'C'; 'D'; 'E'; 'F'; 'G'; 'H'; 'I'; 'J'; 'K'; 'L';
    'M'; 'N'; 'O'; 'P'; 'Q'; 'R'; 'S'; 'T'; 'U'; 'V'; 'W'; 'X'; 'Y'; 'Z'|] in
  Array.sort (fun _ _ -> (Random.int 3) - 1) letters;
  Array.sort (fun _ _ -> (Random.int 3) - 1) letters;
  letters

let is_letter c =
  ((int_of_char c) >= 65) && ((int_of_char c) <= 90)

let crypto str =
  let upper = String.uppercase str in
  let letters = shuffled_letters () in

  (* for each letter in the string *)
  for i = 0 to (String.length upper) - 1 do
    if (is_letter upper.[i]) then begin
      let index = (int_of_char upper.[i]) - 65 in
      upper.[i] <- letters.(index)
    end
  done;
  upper