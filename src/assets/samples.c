#include "assets/samples.h"

TextEntry TEXT_SAMPLES[] =
	{
		// Textos com menos de 30 caracteres
		{
			.text =
			{
				.letters = L"Olá, mundo!",
				.length = 11
			},
			.difficulty = TEXT_DIFFICULTY_EASY
		},
		{
			.text =
			{
				.letters = L"O sol brilha.",
				.length = 13
			},
			.difficulty = TEXT_DIFFICULTY_EASY
		},
		{
			.text =
			{
				.letters = L"A água é azul.",
				.length = 14
			},
			.difficulty = TEXT_DIFFICULTY_EASY
		},
		{
			.text =
			{
				.letters = L"Gatos amam peixe.",
				.length = 17
			},
			.difficulty = TEXT_DIFFICULTY_EASY
		},
		{
			.text =
			{
				.letters = L"Cachorros latem.",
				.length = 16
			},
			.difficulty = TEXT_DIFFICULTY_EASY
		},

		// Textos entre 30 e 100 caracteres
		{
			.text =
			{
				.letters = L"A tecnologia avança rapidamente, mudando nossas vidas de várias maneiras.",
				.length = 73
			},
			.difficulty = TEXT_DIFFICULTY_MEDIUM
		},
		{
			.text =
			{
				.letters = L"O estudo da história nos ajuda a entender o presente e a moldar o futuro de forma mais consciente.",
				.length = 99
			},
			.difficulty = TEXT_DIFFICULTY_MEDIUM
		},
		{
			.text =
			{
				.letters = L"A natureza é uma fonte inesgotável de inspiração e beleza para a humanidade.",
				.length = 76
			},
			.difficulty = TEXT_DIFFICULTY_MEDIUM
		},
		{
			.text =
			{
				.letters = L"A leitura expande nossos horizontes e nos permite viajar para mundos distantes.",
				.length = 78
			},
			.difficulty = TEXT_DIFFICULTY_MEDIUM
		},
		{
			.text =
			{
				.letters = L"A programação é uma habilidade valiosa que abre portas para muitas oportunidades.",
				.length = 79
			},
			.difficulty = TEXT_DIFFICULTY_MEDIUM
		},

		// Textos com mais de 100 caracteres
		{
			.text =
			{
				.letters = L"A inteligência artificial está revolucionando a indústria, automatizando tarefas complexas e criando novas possibilidades para a inovação. No entanto, sua rápida ascensão levanta questões éticas e sociais importantes que precisam ser abordadas.",
				.length = 246
			},
			.difficulty = TEXT_DIFFICULTY_HARD
		},
		{
			.text =
			{
				.letters = L"A globalização conecta economias e culturas em uma rede complexa, facilitando o comércio e a comunicação. No entanto, ela também apresenta desafios, como a necessidade de lidar com a diversidade cultural e a desigualdade econômica em escala global.",
				.length = 249
			},
			.difficulty = TEXT_DIFFICULTY_HARD
		},
		{
			.text =
			{
				.letters = L"O universo é vasto e misterioso, com bilhões de galáxias e estrelas esperando para serem descobertas. A exploração espacial continua a nos fascinar, nos impulsionando a buscar respostas para as grandes questões sobre nossa origem e lugar no cosmos.",
				.length = 245,
			},
			.difficulty = TEXT_DIFFICULTY_HARD
		},
		{
			.text =
			{
				.letters = L"A biologia sintética é uma área promissora que combina biologia e engenharia para criar novas funções biológicas. Essa tecnologia pode ter um impacto significativo na medicina, agricultura e produção de energia, mas também exige uma discussão cuidadosa sobre seus riscos e benefícios.",
				.length = 272,
			},
			.difficulty = TEXT_DIFFICULTY_HARD
		},
		{
			.text =
			{
				.letters = L"O desenvolvimento sustentável é um objetivo crucial para garantir um futuro saudável para as próximas gerações. Isso envolve equilibrar o crescimento econômico com a proteção ambiental e a equidade social, incentivando práticas que reduzem a pegada de carbono e promovem a conservação dos recursos naturais.",
				.length = 298,
			},
			.difficulty = TEXT_DIFFICULTY_HARD
		}
	};

const size_t TEXT_SAMPLES_LENGTH = sizeof(TEXT_SAMPLES) / sizeof(TEXT_SAMPLES[0]);
