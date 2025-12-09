#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>

// Структура приложения
typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
} RulerApp;

// Функция отрисовки линейки
static void ruler_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    
    // Получаем размеры экрана
    uint8_t width = canvas_width(canvas);
    uint8_t height = canvas_height(canvas);
    
    // Экран Flipper Zero: 128x64 пикселя
    // Для 1.4" экрана с разрешением 128x64:
    // Реальная физическая ширина активной области: ~30 мм
    // Калибровка: 128 пикселей / 30 мм = 4.267 пикселей на миллиметр

    // Для масштаба 1:1 используем точный коэффициент
    // 4.267 пикселей = 1 мм, значит 42.67 пикселей ≈ 41 пикселя = 1 см
    const uint8_t pixels_per_cm = 41;
    const uint8_t ruler_y = 30; // Позиция линейки по вертикали
    
    // Заголовок
    canvas_draw_str_aligned(canvas, width / 2, 5, AlignCenter, AlignTop, "RULER");
    canvas_draw_str_aligned(canvas, width / 2, 14, AlignCenter, AlignTop, "1:1 Scale (approx.)");
    
    // Рисуем основную линию линейки
    canvas_draw_line(canvas, 0, ruler_y, width, ruler_y);
    
    // Рисуем сантиметровые деления и миллиметры
    // Используем точное вычисление позиций для устранения накопления погрешности
    const float pixels_per_mm = (float)pixels_per_cm / 10.0f;
    uint8_t cm_count = width / pixels_per_cm;

    for(uint8_t cm = 0; cm <= cm_count; cm++) {
        uint8_t cm_x = cm * pixels_per_cm;
        if(cm_x > width) break;

        // Большое деление для сантиметров
        canvas_draw_line(canvas, cm_x, ruler_y, cm_x, ruler_y + 12);

        // Подпись сантиметров
        if(cm > 0) {
            char label[8];
            snprintf(label, sizeof(label), "%dcm", cm);
            // Выравнивание последнего см по правому краю
            if(cm == cm_count) {
                // Используем canvas_draw_str с ручным смещением для выравнивания по правому краю
                // Ширина текста "3cm" примерно 16 пикселей
                canvas_draw_str(canvas, cm_x - 16, ruler_y + 22, label);
            } else {
                canvas_draw_str(canvas, cm_x - 8, ruler_y + 22, label);
            }
        } else {
            canvas_draw_str(canvas, cm_x, ruler_y + 22, "0");
        }

        // Рисуем миллиметровые деления внутри каждого сантиметра
        // Точное вычисление от начала линейки для устранения погрешности
        for(uint8_t mm = 1; mm < 10; mm++) {
            // Вычисляем позицию от абсолютного начала (0), а не от cm_x
            uint8_t absolute_mm = cm * 10 + mm;
            uint8_t mm_x = (uint8_t)(absolute_mm * pixels_per_mm + 0.5f); // +0.5f для правильного округления
            if(mm_x > width) break;

            // Средние деления для 5 мм (толщина 2 пикселя - добавляем слева)
            if(mm == 5) {
                if(mm_x > 0) {
                    canvas_draw_line(canvas, mm_x - 1, ruler_y, mm_x - 1, ruler_y + 8);
                }
                canvas_draw_line(canvas, mm_x, ruler_y, mm_x, ruler_y + 8);
            } else {
                // Маленькие деления для миллиметров 1-4, 6-9
                canvas_draw_line(canvas, mm_x, ruler_y, mm_x, ruler_y + 4);
            }
        }
    }
    
    // Подсказка внизу
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, width / 2, height - 2, AlignCenter, AlignBottom, "Press Back to exit");
}

// Обработчик ввода
static void ruler_input_callback(InputEvent* input_event, void* context) {
    RulerApp* app = context;
    furi_message_queue_put(app->event_queue, input_event, FuriWaitForever);
}

// Главная функция приложения
int32_t ruler_app(void* p) {
    UNUSED(p);
    
    // Инициализация приложения
    RulerApp* app = malloc(sizeof(RulerApp));
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    
    // Настройка GUI
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, ruler_draw_callback, app);
    view_port_input_callback_set(app->view_port, ruler_input_callback, app);
    
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    
    // Главный цикл
    InputEvent event;
    bool running = true;
    
    while(running) {
        if(furi_message_queue_get(app->event_queue, &event, FuriWaitForever) == FuriStatusOk) {
            // Выход по кнопке Back
            if(event.key == InputKeyBack && event.type == InputTypeShort) {
                running = false;
            }
        }
    }
    
    // Очистка
    gui_remove_view_port(app->gui, app->view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(app->view_port);
    furi_message_queue_free(app->event_queue);
    free(app);
    
    return 0;
}
