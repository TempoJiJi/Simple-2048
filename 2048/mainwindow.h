#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
        void start_game();
        void random_num();
        void game_over();
        bool check_left();
        bool check_right();
        bool check_up();
        bool check_down();
        void keyPressEvent(QKeyEvent *event);
        void setmap();
        void end_game();
        void score_count();
    ~MainWindow();

private slots:
        void on_actionNormal_Mode_triggered();
        void on_actionHandsome_Mode_triggered();
        void on_RButton_clicked();
        void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    int map[16],backup[16],undo[16],score,back_score,mode;
    bool o_left,o_right,o_up,o_down;
};

#endif // MAINWINDOW_H
