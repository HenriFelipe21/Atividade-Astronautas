#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Astronauta {
public:
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;

    Astronauta(string c, int i, string n) {
        cpf = c;
        idade = i;
        nome = n;
        vivo = true;
        disponivel = true;
    }
};

class Voo {
public:
    int codigo;
    vector<string> cpfsPassageiros;
    string estado; 

    Voo(int c) {
        codigo = c;
        estado = "planejado";
    }
};

vector<Astronauta> astronautas;
vector<Voo> voos;

int buscarAstronauta(string cpf) {
    for (int i = 0; i < astronautas.size(); i++) {
        if (astronautas[i].cpf == cpf) return i;
    }
    return -1;
}

int buscarVoo(int codigo) {
    for (int i = 0; i < voos.size(); i++) {
        if (voos[i].codigo == codigo) return i;
    }
    return -1;
}

bool astronautaNoVoo(int iVoo, string cpf) {
    for (string cpfPassageiro : voos[iVoo].cpfsPassageiros) {
        if (cpfPassageiro == cpf) return true;
    }
    return false;
}


int main() {
    string comando;
    while (cin >> comando) {
        if (comando == "FIM") {
            break;
        }
        if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);

            if (buscarAstronauta(cpf) != -1) {
                cout << "Erro: Astronauta com CPF " << cpf << " ja cadastrado.\n";
            } else {
                astronautas.push_back(Astronauta(cpf, idade, nome));
                cout << "Astronauta " << nome << " cadastrado.\n";
            }
        }
        else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;

            if (buscarVoo(codigo) != -1) {
                cout << "Erro: Voo " << codigo << " ja cadastrado.\n";
            } else {
                voos.push_back(Voo(codigo));
                cout << "Voo " << codigo << " cadastrado.\n";
            }
        }
        else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;

            int idAstro = buscarAstronauta(cpf);
            int idVoo = buscarVoo(codigo);

            if (idAstro == -1) cout << "Erro: Astronauta nao encontrado.\n";
            else if (idVoo == -1) cout << "Erro: Voo nao encontrado.\n";
            else if (voos[idVoo].estado != "planejado") cout << "Erro: Voo nao planejado ou ja lancado.\n";
            else if (!astronautas[idAstro].vivo) cout << "Erro: Astronauta morto.\n";
            else if (astronautaNoVoo(idVoo, cpf)) cout << "Erro: Astronauta ja esta no voo.\n";
            else {
                voos[idVoo].cpfsPassageiros.push_back(cpf);
                cout << "Astronauta " << cpf << " adicionado ao voo " << codigo << ".\n";
            }
        }
        else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;

            int idAstro = buscarAstronauta(cpf);
            int idVoo = buscarVoo(codigo);

            if (idAstro == -1) cout << "Erro: Astronauta nao encontrado.\n";
            else if (idVoo == -1) cout << "Erro: Voo nao encontrado.\n";
            else if (voos[idVoo].estado != "planejado") cout << "Erro: Voo nao planejado ou ja lancado.\n";
            else {
                bool removido = false;
                for (int i = 0; i < voos[idVoo].cpfsPassageiros.size(); i++) {
                    if (voos[idVoo].cpfsPassageiros[i] == cpf) {
                        voos[idVoo].cpfsPassageiros.erase(voos[idVoo].cpfsPassageiros.begin() + i);
                        removido = true;
                        cout << "Astronauta " << cpf << " removido do voo " << codigo << ".\n";
                        break;
                    }
                }
                if (!removido) cout << "Erro: Astronauta nao esta nesse voo.\n";
            }
        }
        else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            int idVoo = buscarVoo(codigo);

            if (idVoo == -1) cout << "Erro: Voo nao encontrado.\n";
            else if (voos[idVoo].estado != "planejado") cout << "Erro: Voo nao planejado ou ja lancado.\n";
            else if (voos[idVoo].cpfsPassageiros.empty()) cout << "Erro: Voo sem astronautas.\n";
            else {
                bool podeLancar = true;
                for (string cpf : voos[idVoo].cpfsPassageiros) {
                    int idAstro = buscarAstronauta(cpf);
                    if (!astronautas[idAstro].vivo || !astronautas[idAstro].disponivel) {
                        podeLancar = false;
                        cout << "Erro: Astronauta " << cpf << " morto ou indisponivel. Lancamento cancelado.\n";
                        break;
                    }
                }

                if (podeLancar) {
                    voos[idVoo].estado = "em curso";
                    for (string cpf : voos[idVoo].cpfsPassageiros) {
                        astronautas[buscarAstronauta(cpf)].disponivel = false;
                    }
                    cout << "Voo " << codigo << " lancado.\n";
                }
            }
        }
        else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            int idVoo = buscarVoo(codigo);

            if (idVoo == -1) cout << "Erro: Voo nao encontrado.\n";
            else if (voos[idVoo].estado != "em curso") cout << "Erro: Apenas voos em curso podem explodir.\n";
            else {
                voos[idVoo].estado = "que explodiram";
                for (string cpf : voos[idVoo].cpfsPassageiros) {
                    int idAstro = buscarAstronauta(cpf);
                    astronautas[idAstro].vivo = false;
                    astronautas[idAstro].disponivel = false;
                }
                cout << "Voo " << codigo << " explodiu.\n";
            }
        }
        else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            int idVoo = buscarVoo(codigo);

            if (idVoo == -1) cout << "Erro: Voo nao encontrado.\n";
            else if (voos[idVoo].estado != "em curso") cout << "Erro: Apenas voos em curso podem ser finalizados.\n";
            else {
                voos[idVoo].estado = "finalizados";
                for (string cpf : voos[idVoo].cpfsPassageiros) {
                    astronautas[buscarAstronauta(cpf)].disponivel = true;
                }
                cout << "Voo " << codigo << " finalizado.\n";
            }
        }
        else if (comando == "LISTAR_VOOS") {
            vector<string> estados = {"planejado", "em curso", "finalizados", "que explodiram"};
            
            cout << "\nLISTA DE VOOS:\n";
            for (string estadoBusca : estados) {
                cout << "Voos " << estadoBusca << ":\n";
                for (Voo v : voos) {
                    if (v.estado == estadoBusca) {
                        cout << " -Voo: " << v.codigo << " / Passageiros: ";
                        if (v.cpfsPassageiros.empty()) cout << "Nenhum";
                        else {
                            for (string cpf : v.cpfsPassageiros) {
                                cout << astronautas[buscarAstronauta(cpf)].nome << " (" << cpf << ") ";
                            }
                        }
                        cout << "\n";
                    }
                }
            }
          
        }
        else if (comando == "LISTAR_MORTOS") {
            cout << "\nASTRONAUTAS MORTOS:\n";
            for (Astronauta a : astronautas) {
                if (!a.vivo) {
                    cout << "CPF: " << a.cpf << " / Nome: " << a.nome << " / Voos: ";
                    for (Voo v : voos) {
                        if (v.estado != "planejado" && astronautaNoVoo(buscarVoo(v.codigo), a.cpf)) {
                            cout << v.codigo << " ";
                        }
                    }
                    cout << "\n";
                }
            }
            
        }
    }

    return 0;
}
