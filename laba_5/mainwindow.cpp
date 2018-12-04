#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //при включении узнаем расположение файла лечения
    wchar_t exe_dir[MAX_PATH]; // создаем массив символов под деррикторию
    GetCurrentDirectory(MAX_PATH, exe_dir); // узнаем деррикторию
    ui->Address->setText(QString::fromWCharArray(exe_dir, -1)); // пишем адрес папки в виджет

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Address_Change_clicked() // изменяем адрес папки лечения
{

    ui->Address->setText( // пишем в виджет
        QFileDialog::getExistingDirectory( // результат диалогового окна выбора дерриктории
            this, // родительский виджет для диалогового окна
            tr("Выбор папки востановления"), // название диалога
            ui->Address->text(), // адрес начальной папки диалога
            QFileDialog::ShowDirsOnly // в диалоге показывать только дирректории
            | QFileDialog::DontResolveSymlinks // диалог без использования символических ссылок
        )
    );
}

void MainWindow::on_Start_clicked() // производим лечение
{

    ui->Log->setPlainText(""); // чистим лог

    // проверяем нужно ли востановить отображение расширения файлов
    if (ui->Option_1->isChecked()) {
        HKEY hkey; // переменная указывающая на запись в регистре
        RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced\\", 0, KEY_ALL_ACCESS, &hkey); // задаем ей значение согласно нужной нам записи в реестре
        LPBYTE myPath = new BYTE[4]; // массив байт под содержимое записи
        myPath[0] = 0x00; myPath[1] = 0x00; myPath[2] = 0x00; myPath[3] = 0x00; // формируем то что будем писать в реестр

        RegSetValueEx(hkey, TEXT("HideFileExt"), 0, REG_DWORD, (LPBYTE)myPath, sizeof(myPath)); // пишем в реестр

        HWND hwnd_explorer = GetShellWindow(); // получаем хендл на проводник
        PostMessage(hwnd_explorer, WM_KEYDOWN, VK_F5, 0); // посылаем проводнику нажатие клавиши, что заставит его обновится
    }

    ui->Log->setPlainText(ui->Log->toPlainText() + "Востановлено отображение расширений файлов\n");

    switch (ui->Mode->currentIndex()){

        case 0 :{ // обработка файлов в папке лечения
            // при такой обработке надо найти все файлы txt.exe в дерриктории лечения и заменить их соответствующими txt-файлами если они существуют

            if (GetFileAttributes(L"C:\\Секретная папка с файлами txt") == 0xFFFFFFFF) { // проверяем существует ли наша секретная папка
                // если ее нет, то сообщаем в лог, а все txt.exe-файлы удаляем

                ui->Log->setPlainText(ui->Log->toPlainText() + "Секретной папки не оказалось, txt-файлов для востановления нет\n"); // меняем лог

                WIN32_FIND_DATA find_txtexe_file_data; // структура под список txt.exe-файлов
                HANDLE h_txtexe_file_in_dir; // хендел под очередной txt.exe-файл из дерркиктории
                h_txtexe_file_in_dir = FindFirstFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\*.txt.exe").c_str(), &find_txtexe_file_data); // находим хендел txt.exe-файла из дерриктории

                if (h_txtexe_file_in_dir != INVALID_HANDLE_VALUE) { // если хендел был получен

                    do { // сначала

                        std::wstring find_txtexe_name_wstring = find_txtexe_file_data.cFileName; // берем название txt.exe-файла

                        if (DeleteFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\" + find_txtexe_name_wstring).c_str())) { // удаляем очередной txt.exe-файл
                            ui->Log->setPlainText(ui->Log->toPlainText() + "Удален зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                        }
                        else {
                            ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось удалить зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                        }

                    } while (FindNextFile(h_txtexe_file_in_dir, &find_txtexe_file_data) != 0); // сомтрим хендл
                    FindClose(h_txtexe_file_in_dir); // закрываем хендел txt.exe-файла из дерриктории
                }
            }
            else{
                // если секретная папка есть, то при удалении txt.exe-файла надо проверять можно ли его заменить соответствующим txt-файлом из секретной папки
                WIN32_FIND_DATA find_txtexe_file_data; // структура под список txt.exe-файлов
                HANDLE h_txtexe_file_in_dir; // хендел под очередной txt.exe-файл из дерркиктории
                h_txtexe_file_in_dir = FindFirstFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\*.txt.exe").c_str(), &find_txtexe_file_data); // находим хендел txt.exe-файла из дерриктории

                if (h_txtexe_file_in_dir != INVALID_HANDLE_VALUE) { // если хендел был получен

                    do { // сначала

                        std::wstring find_txtexe_name_wstring = find_txtexe_file_data.cFileName; // берем название txt.exe-файла

                        if (DeleteFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\" + find_txtexe_name_wstring).c_str())) { // удаляем очередной txt.exe-файл
                            ui->Log->setPlainText(ui->Log->toPlainText() + "Удален зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                        }
                        else {
                            ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось удалить зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                        }

                        std::wstring txt_name_wstring = find_txtexe_name_wstring; // строка под навание txt-файла
                        size_t posn; // переменная под позицию последнего вхождения подстроки в строку
                        posn = txt_name_wstring.rfind(L".txt.exe"); // позиция вхождения
                        txt_name_wstring.replace(posn, txt_name_wstring.length(), L".txt"); // формируем название txt-файла

                        if (GetFileAttributes((LPCTSTR)(L"C:\\Секретная папка с файлами txt\\" + txt_name_wstring).c_str()) == 0xFFFFFFFF){ // проверяем существует ли txt-файл в секретной папке
                            //если файла нет сообщаем в лог

                            ui->Log->setPlainText(ui->Log->toPlainText() + "В секретной папке не оказалось txt-файла \"" + QString::fromStdWString(txt_name_wstring) + "\"\n"); // меняем лог

                        } else {
                            //если он существует востанавливаем его

                            if (ui->Option_2->isChecked()){
                                // если txt-файлы при востановлении нужно удалять из секретной папки
                                if (MoveFileEx((L"C:\\Секретная папка с файлами txt\\" + txt_name_wstring).c_str(), (ui->Address->text().toStdWString() + L"\\" + txt_name_wstring).c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) { // перемещаяем очередной txt-файл из секретной папки
                                    ui->Log->setPlainText(ui->Log->toPlainText() + "Востановлен файл \"" + QString::fromStdWString(txt_name_wstring) + "\"\n"); // меняем лог
                                }
                                else {
                                    ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось востановить файл \"" + QString::fromStdWString(txt_name_wstring) + "\"\n"); // меняем лог
                                }
                            }
                            else{
                                //если txt-файлы при востановленни нужно оставлять в секретной папке
                                if (CopyFile((L"C:\\Секретная папка с файлами txt\\" + txt_name_wstring).c_str(), (ui->Address->text().toStdWString() + L"\\" + txt_name_wstring).c_str(), MOVEFILE_REPLACE_EXISTING)) { // перемещаяем очередной txt-файл из секретной папки
                                    ui->Log->setPlainText(ui->Log->toPlainText() + "Скопирован файл \"" + QString::fromStdWString(txt_name_wstring) + "\" из секретной папки\n"); // меняем лог
                                }
                                else {
                                    ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось скопировать файл \"" + QString::fromStdWString(txt_name_wstring) + "\" из секретной папки\n"); // меняем лог
                                }
                            }


                        }

                    } while (FindNextFile(h_txtexe_file_in_dir, &find_txtexe_file_data) != 0); // сомтрим хендл
                    FindClose(h_txtexe_file_in_dir); // закрываем хендел txt.exe-файла из дерриктории
                }
            }

            break;
        }
        case 1 :{ // обработка всех файлов
            // при такой обраобтке удалим все файлы txt.exe в дерриктории лечения и переместим в нее все файлы из секретной папки

            WIN32_FIND_DATA find_txtexe_file_data; // структура под список txt.exe-файлов
            HANDLE h_txtexe_file_in_dir; // хендел под очередной txt.exe-файл из дерркиктории
            h_txtexe_file_in_dir = FindFirstFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\*.txt.exe").c_str(), &find_txtexe_file_data); // находим хендел txt.exe-файла из дерриктории

            if (h_txtexe_file_in_dir != INVALID_HANDLE_VALUE) { // если хендел был получен

                do { // сначала

                    std::wstring find_txtexe_name_wstring = find_txtexe_file_data.cFileName; // берем название txt.exe-файла

                    if (DeleteFile((LPCTSTR)(ui->Address->text().toStdWString() + L"\\" + find_txtexe_name_wstring).c_str())) { // удаляем очередной txt.exe-файл
                        ui->Log->setPlainText(ui->Log->toPlainText() + "Удален зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                    }
                    else {
                        ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось удалить зловред \"" + QString::fromStdWString(find_txtexe_name_wstring) + "\"\n"); // меняем лог
                    }

                } while (FindNextFile(h_txtexe_file_in_dir, &find_txtexe_file_data) != 0); // сомтрим хендл
                FindClose(h_txtexe_file_in_dir); // закрываем хендел txt.exe-файла из дерриктории
            }

            // востанавливаем все txt-файлы

            if (GetFileAttributes(L"C:\\Секретная папка с файлами txt") == 0xFFFFFFFF) { // проверяем существует ли наша секретная папка
                // если ее нет, то сообщаем в лог

                ui->Log->setPlainText(ui->Log->toPlainText() + "Секретной папки не оказалось, txt-файлов для востановления нет\n"); // меняем лог

            }
            else{
                // если папка есть то перемещаем txt-файлы

                WIN32_FIND_DATA find_txt_file_data; // структура под список txt-файлов
                HANDLE h_txt_file_in_dir; // хендел под очередной txt-файл из секретной папки
                std::wstring txt_dir_wstring = L"C:\\Секретная папка с файлами txt"; // адрес секретной папки
                h_txt_file_in_dir = FindFirstFile((LPCTSTR)(txt_dir_wstring + L"\\*.txt").c_str(), &find_txt_file_data); // находим хендел txt.exe-файла из дерриктории

                if (h_txt_file_in_dir != INVALID_HANDLE_VALUE) { // если хендел был получен

                    do { // сначала

                        std::wstring find_txt_name_wstring = find_txt_file_data.cFileName; // берем название txt-файла

                        if (ui->Option_2->isChecked()){
                            // если txt-файлы при востановлении нужно удалять из секретной папки
                            if (MoveFileEx((L"C:\\Секретная папка с файлами txt\\" + find_txt_name_wstring).c_str(), (ui->Address->text().toStdWString() + L"\\" + find_txt_name_wstring).c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) { // перемещаяем очередной txt-файл из секретной папки
                                ui->Log->setPlainText(ui->Log->toPlainText() + "Востановлен файл \"" + QString::fromStdWString(find_txt_name_wstring) + "\"\n"); // меняем лог
                            }
                            else {
                                ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось востановить файл \"" + QString::fromStdWString(find_txt_name_wstring) + "\"\n"); // меняем лог
                            }
                        }
                        else{
                            //если txt-файлы при востановленни нужно оставлять в секретной папке
                            if (CopyFile((L"C:\\Секретная папка с файлами txt\\" + find_txt_name_wstring).c_str(), (ui->Address->text().toStdWString() + L"\\" + find_txt_name_wstring).c_str(), MOVEFILE_REPLACE_EXISTING)) { // перемещаяем очередной txt-файл из секретной папки
                                ui->Log->setPlainText(ui->Log->toPlainText() + "Скопирован файл \"" + QString::fromStdWString(find_txt_name_wstring) + "\" из секретной папки\n"); // меняем лог
                            }
                            else {
                                ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось скопировать файл \"" + QString::fromStdWString(find_txt_name_wstring) + "\" из секретной папки\n"); // меняем лог
                            }
                        }


                    } while (FindNextFile(h_txt_file_in_dir, &find_txt_file_data) != 0); // сомтрим хендл
                    FindClose(h_txt_file_in_dir); // закрываем хендел txt-файла из дерриктории
                }
            }

            break;
        }
        default:{
            QMessageBox::critical( // вывод ошибки
                this,
                tr("Ошибка при обработке"),
                tr("Не удалось провести лечение."),
                0,
                0
            );
            break;
        }
    }

    if(ui->Option_3->isChecked()){ // нужно ли удалить секретную папку если она пуста
        if (!(GetFileAttributes(L"C:\\Секретная папка с файлами txt") == 0xFFFFFFFF)) { // проверяем существует ли наша секретная папка
            WIN32_FIND_DATA find_txt_file_data; // структура под список txt-файлов
            HANDLE h_txt_file_in_dir; // хендел под txt-файл из секретной папки
            std::wstring txt_dir_wstring = L"C:\\Секретная папка с файлами txt"; // адрес секретной папки
            h_txt_file_in_dir = FindFirstFile((LPCTSTR)(txt_dir_wstring + L"\\*.txt").c_str(), &find_txt_file_data); // находим хендел txt.exe-файла из дерриктории

            if (h_txt_file_in_dir == INVALID_HANDLE_VALUE) { // если секретная папка пуста
                if (RemoveDirectory((LPCTSTR)(txt_dir_wstring).c_str())) { // удаляем секретную папку
                    ui->Log->setPlainText(ui->Log->toPlainText() + "Удалена секретная папка, т.к. она пуста\n"); // меняем лог
                }
                else {
                    ui->Log->setPlainText(ui->Log->toPlainText() + "Не удалось удалить секретную папку\n"); // меняем лог
                }
            }
        }
    }
}
