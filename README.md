# FPGA

## Pré-requisitos

Ambiente do oss-cad-suite - https://github.com/YosysHQ/oss-cad-suite-build/releases

Toolchain do riscv64 para seu sistema - [Exemplo](https://github.com/riscv-collab/riscv-gnu-toolchain)

## Descrição

O projeto treina o modelo "hello_world" do TensorFlow, converte para TensorFlow Lite Micro com quantização INT8 e o embute no firmware C. O SoC foi gerado com LiteX usando o core VexRiscv, GPIO mapeado para 8 LEDs externos, e o firmware bare-metal executa inferências periódicas para gerar o efeito de barra de LEDs proporcional ao valor de saída do modelo.

O firmware lê o valor inferido (0 a 255) e converte para um índice de LED, acendendo um LED por vez. O padrão avança e retorna continuamente, criando o efeito de “barra deslizante”.

## Diagrama em texto

```sql
          +-------------------+
          |  TensorFlow Model |
          |  hello_world INT8 |
          +---------+---------+
                    |
                    v
          +-------------------+
          |   TFLM Runtime    |
          |  (Interpreter)    |
          +---------+---------+
                    |
                    v
   +----------------+----------------+
   |       Firmware Bare-Metal       |
   | Loop: inferência → LEDs → wait |
   +----------------+----------------+
                    |
                    v
            +---------------+
            | GPIO / LEDs   |
            +---------------+

```

## Treinamento

```Recomendado:``` Possuir um ambiente venv ou conda rodando para instalar as dependências do python.

Após entrar no ambiente virtual, realize a instalação das dependências.

```bash
pip install tensorflow pandas matplotlib numpy ipykernel
```

Ou (caso do código usado) usar o ambiente do google Colab:

Abra: https://colab.research.google.com

Crie um notebook.

Baixe o arquivo notebook.ipynb e faça o upload no Colab.

```powershell
# Quando o script terminar de rodar, ira gerar uma pasta chamada models, pode acontecer de não gerar o model.cc ou o model_int8.cc, para isso, basta rodar o seguinte comando:

xxd -i model.tflite > model.cc

# Também pode usar .h invés de .cc
```

## Pinos Utilizados (Colorlight i5)

- LEDs externos (8 bits) mapeados em `litex/colorlight_i5.py` como `leds_ext`:
  - `leds_ext[0]` → `P17`
  - `leds_ext[1]` → `P18`
  - `leds_ext[2]` → `N18`
  - `leds_ext[3]` → `L20`
  - `leds_ext[4]` → `L18`
  - `leds_ext[5]` → `G20`
  - `leds_ext[6]` → `M18`
  - `leds_ext[7]` → `N17`
- UART:
  - `serial_tx` → `J17`
  - `serial_rx` → `H18`
- Clock/Reset:
  - `clk25` → `P3`
  - `cpu_reset_n` → `K18`

## Build e Execução

1. Entrar no ambiente de desenvolvimento OSS-CAD-SUITE

```powershell
source [SEU-PATH]/oss-cad-suite/environment
```

2. Gerar/compilar o SoC e o bitstream:

```powershell
python3 litex/colorlight_i5.py --board i9 --revision 7.2 --build --cpu-type=picorv32 --ecppack-compress
```

3. Programar a FPGA com o bitstream de configuração:

```powershell
sudo [SEU-PATH]/oss-cad-suite/bin/openFPGALoader -b colorlight-i5 build/colorlight_i5/gateware/colorlight_i5.bit
```

4. Compilar o TFLM com  o toolchain correto:

```Lembre-se de corrigir o toolchain no firmware/Makefile e em firmware/tflm/Makefile, seu ambiente pode ter versões diferentes```

```powershell
cd firmware/tflm
make

# É recomendado que passe o número de núcleos de cpu do seu computador, no exemplo da produção deste código foi usado 3 (4 núcles disponíveis)

make -j3
```

5. Compilar o firmware:

```powershell
cd .. 
make

# Sair da pasta do firmware

cd ..
```

6. Conectar via terminal serial (litex_term) e carregar o kernel:

```powershell
sudo [SEU-PATH]/oss-cad-suite/bin/litex_term /dev/ttyACM0 --kernel firmware/build/main.bin
```

7. Se depois de rodar o comando acima e não aperecer nada, aperta ENTER e após aparecer **litex>** ou **RUNTIME>** é preciso digitar **reboot** e apertar enter.