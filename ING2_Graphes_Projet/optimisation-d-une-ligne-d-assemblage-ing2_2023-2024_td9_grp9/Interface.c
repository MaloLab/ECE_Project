#include "header.h"

//Page contenant les programmes d'interface
void premierpage(){
    color(9, 0);
    printf("\n ING 2\n");
    color(9, 0);
    printf(" TD 9 ");
    color(9, 0);
    printf("\t\t\t\t\t\t\t\t\t\t\tGAAD Rayan %c\n", 0X01);
    color(9, 0);
    printf("\t\t\t\t\t\t\t\t\t\t\tCOLIN Malo %c\n", 0X01);
    color(9, 0);
    printf("\t\t\t\t\t\t\t\t\t\t\tKERDOUCI Fethi %c\n", 0X01);
    color(9, 0);
    printf("\t\t\t\t\t\t\t\t\t\t\tMELLONE Valentine %c\n", 0X01);
    voiture();

///temporiser le jeu
    Sleep(5000);

}
void color(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}
void voiture()
{
    ///cadeau voiture
    color(0, 9);
    printf("\t\t Optimisation Ligne d'assemblage\n");
    color(9, 0);
    printf(
            "\n"
            "                    ___..................____\n"
            "           _..--''~_______   _____...----~~~\\\\\n"
            "       __.'    .-'~       \\\\~      [_`.%c     \\\\\n"
            " .---~~      .'            \\\\           __..--\\\\_\n"
            "/             `-._          \\\\   _...~~~_..---~  ~~~~~~~~~~~~--.._\n"
            "\\              /  ~~~~~~----_\\`-~_-~~__          ~~~~~~~---.._    ~--.__\n"
            " \\     _       |==            |   ~--___--------...__          `-   _.--\"\"\"|\n"
            "  \\ __/.-._\\   |              |            ~~~~--.  `-._ ___...--~~~_.'|_Y |\n"
            "   `--'|/~_\\\\  |              |     _____           _.~~~__..--~~_.-~~~.-~/\n"
            "     | ||| |\\\\_|__            |.../.----.._.        | Y |__...--~~_.-~  _/\n"
            "      ~\\\\\\ || ~|..__---____   |||||  .'~-. \\\\       |_..-----~~~~   _.~~\n"
            "        \\`-'/ /     ~~~----...|'''|  |/\"_\"\\ \\\\   |~~'           __.~\n"
            "         `~~~'                 ~~-:  ||| ~| |\\\\  |        __..~~\n"
            "                                   ~~|||  | | \\\\/  _.---~~\n"
            "                                     \\\\\\  //  | ~~~\n"
            "                                      \\`-'/  / \n"
            "                                       `~~~~'", 0X01);
}
