//#define EIGENLIB			// uncomment to use Eigen linear algebra library
//#define NO_POINTER_INIT	// uncomment to disable pointer checking

#include "fun_head_fast.h"

// do not add Equations in this area

MODELBEGIN

// insert your equations here, between the MODELBEGIN and MODELEND words

EQUATION("Conveniencia")

	v[0] = VL("Conveniencia", 1);
	v[1] = VL("Marketshare", 1);
 	v[2] = V("tau");  //parametro multiplicador do mercado
 	v[3] = V("cmax"); //conveniencia maxima limite definida pelo mercado
	
	v[4] = (v[0] * (1 - (v[1] * v[2]))) + (v[1] * v[2] * v[3]);
	
	v[5] = VL("Qualidade",1);
	v[6] = V("Qualidade");
	if(v[5] != v[6]) //verifica se teve inovação
	{
		v[4] = V("cmin");
	}
RESULT( v[4] )

EQUATION("Probabilidade")

	v[0] = V("Qualidade"); 
	v[1] = V("aQ");  //relevancia relativa de qualidade
	v[2] = V("Conveniencia");
	v[3] = V("aPI"); //relevancia relativa de preço
	
	v[4] = v[0] * v[1] + v[2] * v[3];

RESULT( v[4] )


EQUATION("Marketshare")

	v[0] = 0.0;
	v[1] = V("Probabilidade");
	CYCLE(cur, "Firmas")
	{
		v[2] = VS(cur, "Probabilidade");
		v[0] = v[0] + v[2];
	}

RESULT( v[1] / v[0] )

EQUATION("Qualidade")

	v[0] = VL("Qualidade", 1);
	v[1] = V("aQ");
	v[2] = VL("Conveniencia", 1);
	v[3] = V("aPI");
	v[4] = V("cmin");
	v[5] = VL("Aprendizado", 1);
	v[7] = uniform(v[0], v[5]);
	if(((v[0] * v[1]) + (v[2] * v[3])) < ((v[7] * v[1]) + (v[4] * v[3]))) //verifica se a probabilidade aumenta
	{
		v[0] = v[7];
	}
	
RESULT( v[0] )


EQUATION("Aprendizado")

	v[0] = VL("Qualidade", 1);
	v[1] = V("aQ");
	v[2] = VL("Conveniencia", 1);
	v[3] = V("aPI");
	v[4] = V("cmin");
	v[5] = VL("Aprendizado", 1);
	v[6] = V("Sigma");
	v[7] = uniform(v[0], v[5]);
	if(((v[0] * v[1]) + (v[2] * v[3])) < ((v[7] * v[1]) + (v[4] * v[3])))//verifica se houve inovação
	{
		v[5] = 0.0;
	}
	else 
	{
		v[5] = v[5] + v[6];
	}

RESULT( v[5])


EQUATION("CMM")

	// Variavel de controle
	v[0] = 0;
	CYCLE(cur, "Firmas")
	{
		// Soma o quadrado de todos os "market share"
		v[1] = VLS(cur,"Marketshare",1);
		v[0] = v[0] + (v[1] * v[1]); 
	}

	// Retorna a razão inversa do somatorio
RESULT( 1 / v[0] )


/*Razão melhor/pior market Share */

EQUATION("RazaoMS")

 	bool primeiraInteracao = true;
	// Passa por todos os elementos "market share"
	CYCLE(cur, "Firmas")
	{
		if(primeiraInteracao)
		{
			// Pega o primeiro valor e o usa como paremetro
			v[0] = VLS(cur,"Marketshare",1);
			v[1] = VLS(cur,"Marketshare",1);
			primeiraInteracao = false;
		}
		else
		{
 		 // Compara todos os valores e pega o maior e menor respectivamente
			v[2] = VLS(cur,"Marketshare",1);
			if(v[2] > v[0])
				v[0] = v[2];
			if(v[2] < v[1])
				v[1] = v[2];
		}
	}

RESULT( v[0] / v[1] )



EQUATION("RazaoQMS") //Razão melhor qualidade pelo market share

// Passando por todas as qualidades para pegar a maior
 	bool primeiraInteracao = true;
	// Passa por todos os elementos "Qualidade"
	CYCLE(cur, "Firmas")
	{
		if(primeiraInteracao)
		{
			// Pega o primeiro valor e o usa como paremetro
			v[0] = VLS(cur,"Qualidade",1);
			primeiraInteracao = false;
		}
		else
		{
  		// Compara todos os valores e pega o maior 
			v[1] = VLS(cur,"Qualidade",1);
			if(v[1] > v[0])
				v[0] = v[1];
		}
	}
	CYCLE(cur, "Firmas")
	{
		v[2] = VLS(cur, "Qualidade", 1);
		if(v[2] == v[0])
			v[3] = VS(cur, "Marketshare"); // Pega o market share atual
	}
// Retorna a razão entre a maior qualidade e o  market share
RESULT( v[0] / v[3])







MODELEND

// do not add Equations in this area

void close_sim( void )
{
	// close simulation special commands go here
}
