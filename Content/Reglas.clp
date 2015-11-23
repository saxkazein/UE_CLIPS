(deftemplate condicion
    (slot id)
)
(deftemplate evento
    (slot id)
)
(defrule showfacts
(salience -10)
(showfacts)
=>
(printout t (facts))
)
(defrule inicio
?f <- (initial-fact)
=>
(retract ?f)
(assert (showfacts))
(assert (condicion (id evento1SinLanzar)))
)
(defrule lanzaEvento1
(condicion (id boton1))
(condicion (id boton2))
(condicion (id boton3))
?f <- (condicion (id evento1SinLanzar
))
=>
(assert (evento (id evento1)))
(retract ?f)
)
