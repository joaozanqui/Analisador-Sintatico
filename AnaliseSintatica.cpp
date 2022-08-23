// João Pedro de Freitas Zanqui
// Sistema operacional: Windows 10
// Compilador: gcc

#include <iostream>
#include <cstdio>
#include <conio.h>

using namespace std;

char proximo = '\0';
int posCodigo = 0;
string codigo;

int linha = 1;
string atomo;

void PROXIMO()
{

    if (posCodigo >= (int)codigo.size())
    {
        proximo = '\0';
        return;
    }

    proximo = codigo[posCodigo++];

    while (proximo == '\t')
        proximo = codigo[posCodigo++];

    while (proximo == '\n')
    {
        proximo = codigo[posCodigo++];
        linha++;
    }
}

bool identificador(string token);

string CODIGO(string atomo)
{
    // : = + * / - , . ; < > ( )
    // program var integer procedure begin if else then div end read while write do
    string codigo;

    if (atomo == ":")
        codigo = "i_doisPontos";
    else if (atomo == "=")
        codigo = "i_igual";
    else if (atomo == ":=")
        codigo = "i_atribuicao";
    else if (atomo == "+")
        codigo = "i_soma";
    else if (atomo == "*")
        codigo = "i_multiplicacao";
    else if (atomo == "/")
        codigo = "i_divisao";
    else if (atomo == "-")
        codigo = "i_subtracao";
    else if (atomo == ",")
        codigo = "i_virgula";
    else if (atomo == ".")
        codigo = "i_ponto";
    else if (atomo == ";")
        codigo = "i_pontoEVirgula";
    else if (atomo == "<")
        codigo = "i_menor";
    else if (atomo == ">")
        codigo = "i_maior";
    else if (atomo == "(")
        codigo = "i_abreParenteses";
    else if (atomo == ")")
        codigo = "i_fechaParenteses";
    else if (atomo == "program")
        codigo = "i_program";
    else if (atomo == "var")
        codigo = "i_var";
    else if (atomo == "integer")
        codigo = "i_integer";
    else if (atomo == "procedure")
        codigo = "i_procedure";
    else if (atomo == "begin")
        codigo = "i_begin";
    else if (atomo == "if")
        codigo = "i_if";
    else if (atomo == "else")
        codigo = "i_else";
    else if (atomo == "then")
        codigo = "i_then";
    else if (atomo == "div")
        codigo = "i_div";
    else if (atomo == "end")
        codigo = "i_end";
    else if (atomo == "read")
        codigo = "i_read";
    else if (atomo == "while")
        codigo = "i_while";
    else if (atomo == "write")
        codigo = "i_write";
    else if (atomo == "do")
        codigo = "i_do";
    else if (identificador(atomo))
        codigo = "i_identificador";
    else
        codigo = "i_numero";

    return codigo;
}

void ERRO(int opc)
{
    cout << "\n\nERRO na linha " << linha << " - ";
    if (opc == 1)
        cout << "Variavel iniciada com digito: " << atomo << endl;
    else
        cout << "Simbolo desconhecido: %c\n"
             << proximo;
}

// -------------------------------------------------------------------------------------------------------------

bool simbolosEspeciais(char caracter)
{
    char caracteres[] = {':', '=', '+', '*', '/', '-', ',', '.', ';', '<', '>', '(', ')'};

    for (int i = 0; i < (int)(sizeof(caracteres) / sizeof(char)); i++)
        if (caracter == caracteres[i])
            return true;

    return false;
}

bool palavrasReservadas(string palavra)
{
    string palavras[] = {"program", "var", "integer", "procedure", "begin", "if", "else", "then", "div", "end", "read", "while", "write", "do"};

    for (int i = 0; i < (int)sizeof(palavras) / sizeof(string); i++)
    {
        if (palavra.compare(palavras[i]) == 0)
            return true;
    }
    return false;
}

bool letras(char letra)
{
    // 65 - 90 , 97 - 122
    int letraInt = (int)letra;
    if ((letraInt >= 65 && letraInt <= 90) || (letraInt >= 97 && letraInt <= 122))
        return true;
    else
        return false;
}

bool digitos(char digito)
{
    // 48 - 57
    int digitoInt = (int)digito;
    if (digitoInt >= 48 && digitoInt <= 57)
        return true;
    else
        return false;
}

// ----------------------------------------- Analisador Lexico - trabalho 1: -----------------------------------------------
int analex()
{

    atomo = "\0";

    if (simbolosEspeciais(proximo))
    {
        atomo.push_back(proximo);
        PROXIMO();

        if (atomo[0] == ':' && proximo == '=')
        {
            atomo.push_back(proximo);
            PROXIMO();
        }
        else if ((atomo[0] == '<' || atomo[0] == '>') && proximo == '=')
        {
            atomo.push_back(proximo);
            PROXIMO();
        }
    }
    else if (letras(proximo))
    {
        do
        {
            atomo.push_back(proximo);
            PROXIMO();
        } while ((letras(proximo) && !palavrasReservadas(atomo)) || digitos(proximo));
    }
    else if (digitos(proximo))
    {
        do
        {
            atomo.push_back(proximo);
            PROXIMO();
        } while (digitos(proximo));

        if (letras(proximo))
        {
            while (proximo != '\0' && !simbolosEspeciais(proximo) && proximo != ' ')
            {
                atomo.push_back(proximo);
                PROXIMO();
            }
            ERRO(1);
            return 0;
        }
    }

    else
    {
        ERRO(2);
        return 0;
    }

    while (proximo == ' ' || proximo == '\t')
        PROXIMO();

    // if (proximo == '\n')
    // {
    //     PROXIMO();
    //     linha++;
    // }

    return 1;
}

// -------------------------------------- Rotinas gramatica Pascal ----------------------------------------------------------------

bool expressao(string atomos[], int *pos);
bool fator(string atomos[], int *pos);
bool listaDeExpressoes(string atomos[], int *pos);
bool comando(string atomos[], int *pos);
bool comandoRepetitivo(string atomos[], int *pos);
bool comandoCondicional(string atomos[], int *pos);
bool comandoComposto(string atomos[], int *pos);
bool desvio(string atomos[], int *pos);
bool chamadaDeProcedimento(string atomos[], int *pos);
bool atribuicao(string atomos[], int *pos);
bool bloco(string atomos[], int *pos);

bool identificador(string token)
{
    if (!letras(token[0]))
        return false;
    for (int i = 1; i < (int)token.length(); i++)
        if ((!letras(token[i]) && !digitos(token[i])) || palavrasReservadas(token))
            return false;
    return true;
}

bool numero(string token)
{
    for (int i = 0; i < (int)token.length(); i++)
        if (!digitos(token[i]))
            return false;
    return true;
}

bool variavel(string token)
{
    return (identificador(token));
}

bool tipo(string token)
{
    return (token == "integer" || token == "real" || token == "string");
}

bool leitura(string atomos[], int *pos)
{
    if (atomos[*pos] != "read")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != "(")
        return false;

    *pos = *pos + 1;

    if (!variavel(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ")")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    return true;
}

bool escrita(string atomos[], int *pos)
{
    if (atomos[*pos] != "write")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != "(")
        return false;

    *pos = *pos + 1;

    if (!variavel(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ")")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    return true;
}

bool listaDeIdentificadores(string atomos[], int *pos)
{
    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    while (atomos[*pos] == ",")
    {
        *pos = *pos + 1;
        if (!identificador(atomos[*pos]))
            return false;
        *pos = *pos + 1;
    }

    return true;
}

bool secaoDeParametrosFormais(string atomos[], int *pos)
{
    if (atomos[*pos] == "var")
        *pos = *pos + 1;

    if (!listaDeIdentificadores(atomos, pos))
        return false;

    if (atomos[*pos] != ":")
        return false;

    *pos = *pos + 1;

    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    return true;
}

bool parametrosFormais(string atomos[], int *pos)
{
    if (!secaoDeParametrosFormais(atomos, pos))
        return false;

    *pos = *pos + 1;

    while (atomos[*pos] == ";")
    {
        *pos = *pos + 1;
        if (!secaoDeParametrosFormais(atomos, pos))
            return false;
        *pos = *pos + 1;
    }

    return true;
}

bool declaracaoDeFuncao(string atomos[], int *pos)
{
    if (atomos[*pos] != "function")
        return false;

    *pos = *pos + 1;

    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    parametrosFormais(atomos, pos);

    if (atomos[*pos] != ":")
        return false;

    *pos = *pos + 1;

    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    if (!bloco(atomos, pos))
        return false;

    return true;
}

bool declaracaoDeProcedimento(string atomos[], int *pos)
{
    if (atomos[*pos] != "procedure")
        return false;

    *pos = *pos + 1;

    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    parametrosFormais(atomos, pos);

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    if (!bloco(atomos, pos))
        return false;

    return true;
}

bool parteDeDeclaracoesDeSubRotinas(string atomos[], int *pos)
{
    if (!declaracaoDeProcedimento(atomos, pos) && !declaracaoDeFuncao(atomos, pos))
    {
        return false;
        // cout << "subrotinas - " << atomos[*pos] << endl;
        // *pos = *pos + 1;

        // if (atomos[*pos] != ";")
        //     return false;

        // *pos = *pos + 1;
    }

    return true;
}

bool declaracaoDeVariaveis(string atomos[], int *pos)
{
    if (!listaDeIdentificadores(atomos, pos))
        return false;

    if (atomos[*pos] != ":")
        return false;

    *pos = *pos + 1;

    if (!tipo(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    return true;
}

bool parteDeDeclaracoesDeVariaveis(string atomos[], int *pos)
{
    if (atomos[*pos] != "var")
        return false;

    *pos = *pos + 1;

    if (!declaracaoDeVariaveis(atomos, pos))
        return false;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    while (declaracaoDeVariaveis(atomos, pos))
    {

        if (atomos[*pos] != ";")
            return false;

        *pos = *pos + 1;
    }

    return true;
}

bool parteDeDeclaracoesDeRotulos(string atomos[], int *pos)
{
    if (atomos[*pos] != "label")
        return false;

    *pos = *pos + 1;

    if (!numero(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    while (atomos[*pos] == ",")
    {
        *pos = *pos + 1;

        if (!numero(atomos[*pos]))
            return false;

        *pos = *pos + 1;
    }

    return true;
}

bool relacao(string atomos[], int *pos)
{
    //  = | <> | < | <= | >= | >
    if (atomos[*pos] == "=" || atomos[*pos] == "<>" || atomos[*pos] == "<" || atomos[*pos] == "<=" || atomos[*pos] == ">=" || atomos[*pos] == ">")
    {
        *pos = *pos + 1;
        return true;
    }
    return false;
}

bool bloco(string atomos[], int *pos)
{
    parteDeDeclaracoesDeRotulos(atomos, pos);
    parteDeDeclaracoesDeVariaveis(atomos, pos);
    parteDeDeclaracoesDeSubRotinas(atomos, pos);

    if (!comando(atomos, pos))
        return false;

    return true;
}

bool comandoSemRotulo(string atomos[], int *pos)
{
    //<atribuição> | <chamada de procedimento> | <desvio>| <comando composto> |<comando condicional> |<comando repetitivo>
    if (atribuicao(atomos, pos) || chamadaDeProcedimento(atomos, pos) || escrita(atomos, pos) || leitura(atomos, pos) || comandoRepetitivo(atomos, pos) || comandoCondicional(atomos, pos) || desvio(atomos, pos) || comandoComposto(atomos, pos))
        return true;
    return false;
}

bool comandoComposto(string atomos[], int *pos)
{
    if (atomos[*pos] != "begin")
        return false;

    *pos = *pos + 1;

    if (!comando(atomos, pos))
        return false;

    while (atomos[*pos - 1] == ";" && atomos[*pos] != "end")
    {
        if (!comando(atomos, pos))
            return false;
    }

    if (atomos[*pos] != "end")
        return false;

    *pos = *pos + 1;

    return true;
}

bool comando(string atomos[], int *pos)
{
    if (numero(atomos[*pos]))
    {
        *pos = *pos + 1;

        if (atomos[*pos] != ":")
            return false;
    }

    if (!comandoSemRotulo(atomos, pos))
        return false;

    return true;
}

bool desvio(string atomos[], int *pos)
{
    if (atomos[*pos] != "goto")
        return false;

    *pos = *pos + 1;

    if (!numero(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    return true;
}

bool atribuicao(string atomos[], int *pos)
{
    if (!variavel(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ":=")
    {
        *pos = *pos - 1;
        return false;
    }

    *pos = *pos + 1;

    if (!expressao(atomos, pos))
        return false;

    // if (variavel(atomos[*pos + 1]))
    // {
    //     *pos = *pos + 1;
    //     return atribuicao(atomos, pos);
    // }

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    return true;
}

bool chamadaDeProcedimento(string atomos[], int *pos)
{
    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    // listaDeExpressoes(atomos, pos);

    if (atomos[*pos] != ";")
    {
        *pos = *pos - 1;
        return false;
    }

    *pos = *pos + 1;

    return true;
}

bool comandoCondicional(string atomos[], int *pos)
{
    if (atomos[*pos] != "if")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != "(")
        return false;

    *pos = *pos + 1;

    if (!expressao(atomos, pos))
        return false;

    if (atomos[*pos] != ")")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != "then")
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] == "begin")
        comandoComposto(atomos, pos);

    if (!comandoSemRotulo(atomos, pos))
        return false;

    if (atomos[*pos] == "else")
    {
        *pos = *pos + 1;

        if (!comandoSemRotulo(atomos, pos))
            return false;
    }

    return true;
}

bool comandoRepetitivo(string atomos[], int *pos)
{
    if (atomos[*pos] != "while")
        return false;

    *pos = *pos + 1;

    if (!expressao(atomos, pos))
        return false;

    if (atomos[*pos] != "do")
        return false;

    *pos = *pos + 1;

    if (!comandoComposto(atomos, pos))
        return false;

    return true;
}

bool termo(string atomos[], int *pos)
{
    bool parenteses = false;
    if (atomos[*pos] == "(")
    {
        *pos = *pos + 1;
        parenteses = true;
    }

    if (!fator(atomos, pos))
        return false;

    while (atomos[*pos] == "*" || atomos[*pos] == "div" || atomos[*pos] == "and")
    {
        *pos = *pos + 1;
        if (!fator(atomos, pos))
            return false;
    }

    if (parenteses)
    {
        if (atomos[*pos] != ")")
            return false;
        *pos = *pos + 1;
    }

    if (atomos[*pos] == "*" || atomos[*pos] == "div" || atomos[*pos] == "and")
    {
        *pos = *pos + 1;
        if (!termo(atomos, pos))
            return false;
    }

    return true;
}

bool expressaoSimples(string atomos[], int *pos)
{
    if (atomos[*pos] == "+" || atomos[*pos] == "-")
    {
        *pos = *pos + 1;
    }

    if (!termo(atomos, pos))
        return false;

    while (atomos[*pos] == "+" || atomos[*pos] == "-" || atomos[*pos] == "or")
    {
        *pos = *pos + 1;
        if (!termo(atomos, pos))
            return false;
    }

    return true;
}

bool expressao(string atomos[], int *pos)
{
    if (!expressaoSimples(atomos, pos))
        return false;

    if (relacao(atomos, pos))
    {
        if (!expressaoSimples(atomos, pos))
            return false;
    }

    return true;
}

bool listaDeExpressoes(string atomos[], int *pos)
{
    if (!expressao(atomos, pos))
        return false;

    while (atomos[*pos] == ",")
    {
        *pos = *pos + 1;

        if (!expressao(atomos, pos))
            return false;
    }

    return true;
}

bool chamadaDeFuncao(string atomos[], int *pos)
{
    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    listaDeExpressoes(atomos, pos);

    return true;
}

bool fator(string atomos[], int *pos)
{
    //<variável> | <número> | <chamada de função> | (<expressão>) | not<fator>
    if (variavel(atomos[*pos]) || numero(atomos[*pos]) || chamadaDeFuncao(atomos, pos) || expressao(atomos, pos) || !fator(atomos, pos))
    {
        *pos = *pos + 1;
        return true;
    }
    return false;
}

bool programa(string atomos[], int *pos)
{
    if (atomos[*pos] != "program")
        return false;

    *pos = *pos + 1;

    if (!identificador(atomos[*pos]))
        return false;

    *pos = *pos + 1;

    if (atomos[*pos] != ";")
        return false;

    *pos = *pos + 1;

    if (!bloco(atomos, pos))
        return false;

    if (atomos[*pos] != ".")
        return false;

    return true;
}

bool analiseSintatica(string atomos[], int *pos)
{
    if (!programa(atomos, pos))
    {
        posCodigo = 0;
        linha = 1;

        PROXIMO();

        for (int i = 0; i < *pos; i++)
            analex();

        return false;
    }

    return true;
}

int main()
{
    string atomos[500];
    int qtdAtomos = 0;
    // -------------------  ------------------- leitura do arquivo ----------------------------------------------------------------

    FILE *arquivo;
    char codigoLeitura[500];

    for (int i = 0; i < 500; i++)
        codigoLeitura[i] = '\0';

    arquivo = fopen("Exemplo1_Trab2_Compiladores.txt", "r");
    fread(codigoLeitura, sizeof(char), 500, arquivo);
    fclose(arquivo);

    codigo = codigoLeitura;

    while (proximo == '\0')
        PROXIMO();

    // Armazena todos os atomos em um vetor
    for (int i = 0; proximo != '\0'; i++)
    {
        if (analex())
        {
            atomos[i] = atomo;
            qtdAtomos++;
        }
    }

    // for (int i = 0; i < qtdAtomos; i++)
    //     cout << atomos[i] << endl;

    int pos = 0;

    if (analiseSintatica(atomos, &pos))
        cout << "Analise Sintatica realizada com sucesso" << endl;
    else
        cout << "ERRO NA LINHA " << linha << endl
             << "Atomo: " << atomos[pos] << endl;
    return 0;
}