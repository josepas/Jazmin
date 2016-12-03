func HeronRC(float n) -> float {

	float tmp
	float estimado = n / 2

	while ( n - estimado*estimado > 0.0001) {
		tmp = n / estimado 
		estimado = (tmp + estimado) / 2
	}

	return estimado
}

program {
	
	char opcion
	float n

	write "Este programa aproxima raices cuadradas"
	while (True) {
		write "Ingrese el número a procesar"
		read n
		if (n < 0) {
			write "Que no sea negativo"
			continue
		}
		write HeronRC(n)

		write "Desea continuar (s/n)"
		read opcion

		if (opcion != 's') {
			write "No dijiste que si, soy flojo"
			break
		}
	}

}