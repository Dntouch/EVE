# Integrationsbericht 024 – Forschungsstand B.5.1

**Stand:** 18. November 2026  
**Ziel:** Konsolidierung von B.4.3 bis B.5.1 mit `main`  
**Integrationsbranch:** `integration/research-b51`  
**Quellbranch:** `codex/research-stage-b51-natural-learning-stability`  
**Zielbranch:** `main`

## Bestandsaufnahme

Die Remote-Referenzen wurden vor der Integration mit `git fetch --prune origin`
aktualisiert; ein `pull` wurde nicht ausgeführt. Der Arbeitsbaum war vor Beginn
sauber.

- `origin/main`: `69044a2`
- B.5.1-Quellstand: `c7b6fa7`
- gemeinsamer Ausgangspunkt: `45009e9`
- nur auf `main`: 5 Commits
- nur auf dem Forschungsbranch: 41 Commits

Die Integration erfolgte als echter Merge auf einem von `origin/main`
abgeleiteten Branch. Die Forschungshistorie wurde weder gesquasht noch
umgeschrieben.

## Konfliktauflösung

Drei Dateien erforderten eine inhaltliche Auflösung:

1. `CHANGELOG.md`: Risikoregister und Prozessdokumentation aus `main` wurden mit
   den Forschungsstufen bis B.5.1 kombiniert.
2. `README.md`: Der aktuelle Projektstatus wurde auf B.5.1 angehoben. Der
   Meilenstein 001 aus `main` bleibt erhalten; Forschungshistorie, B.5.1-Runner
   und lokale Dokumentlinks stammen aus dem Forschungszweig.
3. `docs/konzept/EVE-Stammhirnkonzept.md`: Das vollständige Stammhirnkonzept aus
   `main` wurde um die später dokumentierten Forschungsstufen und Verweise
   ergänzt.

Zusätzlich wurden `SECURITY.md`, `docs/RISIKEN.md` und die historische
Entscheidung 0002 aus `main` bewusst erhalten. Die Entscheidung 0002 wird im
Index als durch die spätere C++-Implementierung überholter historischer Stand
eingeordnet. `docs/konzept/EVE-Konzept.md` und
`docs/meilensteine/001-erste-erlernte-faehigkeit.md` blieben unverändert
erhalten. Es wurden keine Forschungsdaten gelöscht.

## Technische Validierung

### Sauberer Make-Build

- Werkzeug: GCC 16.2.1
- Befehle: `make clean`, `make all`, `make test`
- Ergebnis: alle Basis-, Experiment- und Forschungs-Binaries bis B.5.1 gebaut
- Regression: **509/509 Tests bestanden**

Die Testsuite umfasst insbesondere deterministische Speicherung und
Wiederherstellung, Kompatibilität der bestehenden Zustandskonfigurationen,
vollständige Zustandsfingerabdrücke, synaptische Gewichtsgrenzen, gültige
Netzarchitekturen sowie die Experimentkonfigurationen B.4 bis B.5.1.

### Sauberer CMake-Build

Auf dem Host war CMake nicht installiert. Deshalb wurde der Pflichtlauf in
einem bereits lokal vorhandenen Entwicklungscontainer mit schreibgeschütztem
Repository-Mount und isoliertem Build-Verzeichnis ausgeführt.

- CMake 3.31.4, GNU C++ 11.4.0, Release-Konfiguration
- Konfiguration und Erzeugung erfolgreich
- alle Targets zu 100 % gebaut
- CTest: 1/1 Testziel bestanden; dieses Testziel führt intern dieselben 509
  Einzeltests aus

### B.5.1-Kontrollreproduktion

Der B.5.1-Runner wurde erneut mit einem getrennten temporären Ergebnisverzeichnis
unter `/tmp` ausgeführt. Historische Daten wurden nicht überschrieben.

- Abschlussmeldung: `B.5.1 natural learning lifecycle complete K9=not_assumed`
- Dateisatz-Hash ohne Performance-Dateien:
  `9db69e9e26d94dccedbe45a53cc0b4b7c61489550402c10d90cf9a5e610e9284`
- alle deterministischen Dateien stimmen bytegenau mit
  `research/results/b51` überein
- `performance.csv` und `b50-reproduction/performance.csv` wurden wegen ihrer
  Rechner- und Lastabhängigkeit nicht bytegenau verglichen
- alle 31 vorhandenen `.gz`-Ergebnisdateien bestanden die Integritätsprüfung

## Wissenschaftliche Einordnung

Die Integration ändert keine Lernmechanismen und keine Versuchsergebnisse.
B.5.1 weist natürlichen Erwerb und Wiedererwerb der untersuchten Pfade nach,
aber keinen stabilen gleichzeitigen Erhalt konkurrierender Beziehungen.
Inhibitorische Plastizität bleibt experimentell; adaptive Zusatzexploration,
allgemeines Online-Lernen und neue Stabilisierungsmechanismen bleiben gesperrt.

Eine erfolgreiche technische Implementierung wird nicht als Nachweis
allgemeiner neuronaler Intelligenz bewertet.

## Offene Forschung für B.5.2

- inhibitorische Eligibility nichtursächlicher Kandidaten,
- Interferenz gemeinsam veränderter sensorischer Synapsen,
- Einfluss transienter Membran- und Erholungszustände,
- Unterscheidung zwischen tatsächlichem Verlust gespeicherter Verbindungen und
  vorübergehend gestörtem Abruf.

Dieser Integrationsbericht implementiert keine neue Lernfunktion.
