////////////////////////////////////////////////////////////////////////////////
// OPÇÕES DISPNÍVEIS NOS MENUS
////////////////////////////////////////////////////////////////////////////////
int NavigateMenuIndex = 0;

String TextoBranco = "                ";

String MainMenuOptions[] = { 
  "Prog Horario  ",
  "Ligar Agora   ",
  "Reset Config  ",
  "Ler Config    ",
  "Salvar Config "
};

bool CheckProgHorarioAccess = false;
String ProgHorarioOptions[] = { 
  "Config Relogio",
  "Prog Horario 1",
  "Prog Horario 2",
  "Voltar        "
};

bool CheckConfigRelogioAccess = false;
String ConfigRelogioOptions[] = { 
  "Definir Hora  ",
  "Definir Min   ",
  /*"Definir Dia   ",
  "Definir Mes   ",
  "Definir Ano   ",*/
  "Voltar        "
};

bool CheckProgHorarioSet1Access = false;
bool CheckProgHorarioSet2Access = false;
String ProgHorarioSetOptions[] = { 
  "Ativar?       ",
  "Tempo(s)      ",
  "Prog Hora     ",
  "Prog Min      ",
  "Voltar        "
};

bool CheckAlimentarAgoraAccess = false;
String ProgAlimentarAgoraOptions[] = { 
  "Tempo(s)      ",
  "Ligar         ",
  "Voltar        "
};