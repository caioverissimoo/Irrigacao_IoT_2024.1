# Irrigacao_IoT_2024.1
Repositório para o trabalho final da matéria de IoT 2024.1

## Descrição do Projeto
Este projeto visa implementar um sistema de irrigação inteligente utilizando tecnologias de IoT na Fazenda Fittipaldi, localizada em Petrolina, Pernambuco. A proposta inclui a utilização de sensores de umidade e temperatura do solo para monitoramento e controle eficiente da irrigação.

## Links Importantes
- **Apresentação**: [Visualizar Apresentação](https://drive.google.com/file/d/1MH5PPSzFCF-W0WzXZbT547MrWPnAaaAy/view?usp=sharing)
- **Código do Sensor**: [Acessar Código do Sensor](https://github.com/caioverissimoo/Irrigacao_IoT_2024.1/tree/main/lorawan_at_slave)
- **Código do Nó**: [Acessar Código do Nó](https://github.com/caioverissimoo/Irrigacao_IoT_2024.1/tree/main/lorawan_end_node)
- **Relatório**: [Visualizar Relatório](https://github.com/caioverissimoo/Irrigacao_IoT_2024.1/blob/main/relatorio.pdf)

## Simulação
A simulação utilizada neste projeto foi reaproveitada de um trabalho de referência, adaptada para atender às necessidades específicas da Fazenda Fittipaldi.
- **Repositório de Referência**: [Visualizar Referência](https://github.com/umjourje/CPS730-IoT-2023_3)

## Estrutura do Projeto
O projeto é dividido nas seguintes partes:
1. **Sensores**: Implementação dos sensores de umidade e temperatura do solo.
2. **Nó IoT**: Configuração do nó IoT para coleta e transmissão dos dados.
3. **Rede e Infraestrutura**: Descrição da estrutura de rede utilizada, incluindo camadas de borda, intermediária e nuvem.
4. **Visualização dos Dados**: Painel de controle para visualização dos dados em tempo real e histórico.

## Dashboard
O Dashboard foi implementado na plataforma **LORIOT.io**, em conjunto com o ambiente **STM32CUBEMONITOR**. 

### Versão Atual
- **Fluxo de Comunicação Uplink**: Sensores -> Nó de Controle.
- **Nós de Injeção**: Utilizados para simular dados reais.

### Próximas Versões
- **Comunicação Downlink**: Adicionar a capacidade de envio de comandos do nó de controle para os sensores.
- **Conexão com Sensores Reais**: Integrar sensores reais ou ambiente de simulação para exibir dados reais.
- **Substituição do Nó de Injeção**: Implementar dados reais no dashboard, substituindo os nós de injeção atuais.
