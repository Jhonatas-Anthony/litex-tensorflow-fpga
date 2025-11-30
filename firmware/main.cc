extern "C" {
    #include <generated/csr.h>
    #include <uart.h>
    #include <irq.h>
}

// #include <math.h>

// #include "tflm/tensorflow/lite/micro/micro_interpreter.h"
// #include "tflm/tensorflow/lite/micro/micro_mutable_op_resolver.h"
// #include "tflm/tensorflow/lite/schema/schema_generated.h"
// #include "tflm/examples/hello_world/models/hello_world_int8_model_data.h"

static const int arena_size = 3024;
static uint8_t arena[arena_size];

extern "C" void isr(void) {}

// static void putchar_uart(char c) {
//     while (uart_txfull_read());
//     uart_rxtx_write(c);
// }

// static void puts_uart(const char* s) {
//     while (*s) putchar_uart(*s++);
// }

#ifdef CSR_LEDS_OUT_ADDR
static inline void leds_write_u8(uint8_t v) {
    leds_out_write(v);
}
#else
static inline void leds_write_u8(uint8_t) {}
#endif

// Converte valor [-1,1] para padrão de LEDs 0..8
// static uint8_t leds_from_value(float v) {
//     int n = (int)((v + 1.f) * 4.f + 0.5f);
//     if (n < 0) n = 0;
//     if (n > 8) n = 8;

//     uint8_t mask = 0;
//     for (int i = 0; i < n; i++)
//         mask |= (1u << i);

//     return mask;
// }

int main() {

    uart_init();

    // Teste rápido dos LEDs
    for (int i = 0; i < 256; i++) {
        leds_write_u8(1u << i);
        for (volatile int d = 0; d < 80000; d++);
    }
    leds_write_u8(0);

    // // Carrega modelo
    // const tflite::Model* model = tflite::GetModel(g_hello_world_int8_model_data);
    // if (model->version() != TFLITE_SCHEMA_VERSION) {
    //     puts_uart("Modelo incorreto\n");
    //     while (1);
    // }

    // // Prepara operadores usados pelo modelo
    // tflite::MicroMutableOpResolver<1> ops;
    // ops.AddFullyConnected();

    // // Cria interpretador
    // tflite::MicroInterpreter interp(model, ops, arena, arena_size);
    // if (interp.AllocateTensors() != kTfLiteOk) {
    //     puts_uart("Falha nos tensores\n");
    //     while (1);
    // }

    // TfLiteTensor* input  = interp.input(0);
    // TfLiteTensor* output = interp.output(0);

    // int counter = 0;
    // const float two_pi = 6.28318530718f;

    // while (1) {

    //     // gera seno inteiro quantizado conforme entrada do modelo
    //     float x = (counter * two_pi) / 200.f;
    //     float s = sinf(x);

    //     int8_t q_in = (int8_t)lrintf(s / input->params.scale + input->params.zero_point);
    //     input->data.int8[0] = q_in;

    //     // executa a IA
    //     if (interp.Invoke() != kTfLiteOk) {
    //         puts_uart("Erro na inferencia\n");
    //         while (1);
    //     }

    //     // lê saída e dequantiza
    //     int8_t q_out = output->data.int8[0];
    //     float y = (q_out - output->params.zero_point) * output->params.scale;

    //     // LEDs proporcional ao valor
    //     leds_write_u8(leds_from_value(y));

    //     // passo temporal
    //     for (volatile int t = 0; t < 30000; t++);

    //     counter++;
    //     if (counter >= 200) counter = 0;
    // }

    return 0;
}
