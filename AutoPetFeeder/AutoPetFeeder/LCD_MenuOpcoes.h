////////////////////////////////////////////////////////////////////////////////
// OPÇÕES DISPNÍVEIS NOS MENUS
////////////////////////////////////////////////////////////////////////////////
int NavigateMenuIndex = 0;

static String MainMenuOptions[] = { 
  "Prog Horario  ",
  "Ligar Agora   ",
  "Reset Config  ",
  "Ler Config    ",
  "Salvar Config "
};

bool CheckProgHorarioAccess = false;
static String ProgHorarioOptions[] = { 
  "Config Relogio",
  "Prog Horario 1",
  "Prog Horario 2",
  "Voltar        "
};

bool CheckConfigRelogioAccess = false;
static String ConfigRelogioOptions[] = { 
  "Def. Hora     ",
  "Def. Min      ",
  "Def. Dia      ",
  "Def. Mes      ",
  "Def. Ano      ",
  "Voltar        "
};

bool CheckProgHorarioSet1Access = false;
bool CheckProgHorarioSet2Access = false;
static String ProgHorarioSetOptions[] = { 
  "Ativar?       ",
  "Tempo(s)      ",
  "Prog Hora     ",
  "Prog Min      ",
  "Voltar        "
};

bool CheckAlimentarAgoraAccess = false;
static String ProgAlimentarAgoraOptions[] = { 
  "Tempo(s)      ",
  "Ligar         ",
  "Voltar        "
};
