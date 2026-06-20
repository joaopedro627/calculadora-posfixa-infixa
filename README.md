# calculadora-posfixa-infixa

Desenvolvi esta calculadora matemática em C como um projeto acadêmico de Engenharia de Software na Universidade Católica de Brasília. O sistema é capaz de processar expressões matemáticas avançadas e realizar a conversão estrutural para a notação infixa. A arquitetura principal foi construída utilizando a estrutura de dados Pilha, gerenciando fluxos independentes para valores numéricos e tokens de strings.

A maior barreira técnica não foi a sintaxe do C, mas sim a implementação de  um sistema que lidasse com os parenteses das expressoes. Para garantir que a formatação infixa ficasse matematicamente correta e limpa, precisei mapear uma hierarquia de precedência para os operadores e criar variáveis de controle que decidissem dinamicamente quando encapsular uma expressão em parênteses era obrigatório.

O desenvolvimento desse projeto me provou o quanto um bom planejamento algorítmico economiza tempo de código. Ao desvendar e estruturar a lógica da precedência dos operadores primeiro, as funções de avaliação e montagem se resolveram de forma surpreendentemente simples. A experiência consolidou meu domínio prático sobre Pilhas e sobre o design de lógica condicional para parsing de strings.
