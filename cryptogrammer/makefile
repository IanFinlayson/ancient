# makefile for cryptogrammer
# 
# cryptogrammer: gui.ml
# 	ocamlc -I +lablgtk2 -o cryptogrammer lablgtk.cma gtkInit.cmo gui.ml

cryptogrammer: logic.cmo gui.cmo
	ocamlc -I +lablgtk2 -o cryptogrammer lablgtk.cma gtkInit.cmo logic.cmo gui.cmo

gui.cmo: gui.ml logic.ml
	ocamlc -c -I +lablgtk2 gui.ml

logic.cmo: logic.ml
	ocamlc -c logic.ml

clean:
	rm -f *.cmo *.cmi cryptogrammer
	chmod -x *.ml makefile

execute:
	./cryptogrammer
	
