"""Complete portable generator for Tickform Phase 4 PCM1808 C KiCad 10 schematic project (Rev R0.1).
Checkpoint documentale e rettifiche CHAT OVERVIEW (13/11/2026):
1. Percorso relativo portabile (os.path.dirname).
2. UUID deterministici stabili (uuid.uuid5 con namespace fisso).
3. Precisione metrologica corretta: '96,000 kHz rilevati tramite cursori PulseView'.
4. Misura reale scheda C: AGND chip pin 2 -> GND 2 breakout circa 0,2 ohm.
5. Strumento passivo INSTR1: compare nello schema grafico ma escluso dalla netlist PCB (on_board no / in_bom no).
"""
import os
import uuid
import json

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OUTPUT_DIR = SCRIPT_DIR
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Deterministic UUID generator to ensure reproducible files without random diffs
NAMESPACE = uuid.UUID("d480a719-75c1-4822-b91c-196d4b29a001")
uuid_counter = 0

def det_uuid(name=None):
    global uuid_counter
    uuid_counter += 1
    key = name if name is not None else f"item_{uuid_counter}"
    return str(uuid.uuid5(NAMESPACE, key))

# 1. kicad_pro
pro = {
  "board": {
    "design_settings": {
      "defaults": {},
      "diff_pair_dimensions": [],
      "drc_exclusions": [],
      "rules": {},
      "track_widths": [],
      "via_dimensions": []
    }
  },
  "boards": [],
  "libraries": {
    "pinned_footprint_libs": [],
    "pinned_symbol_libs": []
  },
  "meta": {
    "filename": "tickform_phase4_pcm1808c.kicad_pro",
    "version": 1
  },
  "net_settings": {
    "classes": [],
    "meta": {
      "version": 0
    }
  },
  "pcbnew": {
    "page_layout_descr_file": ""
  },
  "sheets": [],
  "text_variables": {}
}

with open(os.path.join(OUTPUT_DIR, "tickform_phase4_pcm1808c.kicad_pro"), "w", encoding="utf-8") as f:
    json.dump(pro, f, indent=2)

# 2. sym-lib-table
sym_lib_table = """(sym_lib_table
  (version 7)
  (lib (name "tickform_symbols")(type "KiCad")(uri "${KIPRJMOD}/tickform_symbols.kicad_sym")(options "")(descr "Tickform Phase 4 Local Symbols"))
)
"""
with open(os.path.join(OUTPUT_DIR, "sym-lib-table"), "w", encoding="utf-8") as f:
    f.write(sym_lib_table)

# 3. tickform_symbols.kicad_sym
sym_lib = """(kicad_symbol_lib
	(version 20241209)
	(generator "kicad_symbol_editor")
	(generator_version "10.0")
	(symbol "WeAct_STM32G431_Header"
		(pin_names (offset 1.016))
		(in_bom yes)
		(on_board yes)
		(property "Reference" "U" (at 0 10.16 0) (effects (font (size 1.27 1.27))))
		(property "Value" "WeAct_STM32G431_Header" (at 0 -10.16 0) (effects (font (size 1.27 1.27))))
		(property "Footprint" "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" (at 0 -12.7 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "WeAct Studio V1.0 STM32G431 Core Board - Header destro (pin 1..4)" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "WeAct_STM32G431_Header_0_1"
			(rectangle (start -15.24 7.62) (end 7.62 -7.62) (stroke (width 0.254) (type default)) (fill (type background)))
			(text "WeAct Studio V1.0" (at -3.81 5.08 0) (effects (font (size 1.016 1.016) (bold yes))))
			(text "STM32G431CBU6" (at -3.81 2.54 0) (effects (font (size 1.016 1.016))))
			(text "Fila Destra (1..4)" (at -3.81 0 0) (effects (font (size 1.016 1.016) (italic yes))))
			(text "USB-C In Alto (PC)" (at -3.81 -2.54 0) (effects (font (size 0.889 0.889))))
			(text "Alimentazione 5V" (at -3.81 -5.08 0) (effects (font (size 0.889 0.889))))
		)
		(symbol "WeAct_STM32G431_Header_1_1"
			(pin power_out line (at 12.7 3.81 180) (length 5.08) (name "3V3" (effects (font (size 1.27 1.27)))) (number "1" (effects (font (size 1.27 1.27)))))
			(pin power_out line (at 12.7 1.27 180) (length 5.08) (name "GND" (effects (font (size 1.27 1.27)))) (number "2" (effects (font (size 1.27 1.27)))))
			(pin bidirectional line (at 12.7 -1.27 180) (length 5.08) (name "B11" (effects (font (size 1.27 1.27)))) (number "3" (effects (font (size 1.27 1.27)))))
			(pin power_out line (at 12.7 -3.81 180) (length 5.08) (name "V+" (effects (font (size 1.27 1.27)))) (number "4" (effects (font (size 1.27 1.27)))))
		)
	)
	(symbol "XO_24M576_DIP14"
		(pin_names (offset 1.016))
		(in_bom yes)
		(on_board yes)
		(property "Reference" "U" (at 0 7.62 0) (effects (font (size 1.27 1.27))))
		(property "Value" "XO_24M576_DIP14" (at 0 -7.62 0) (effects (font (size 1.27 1.27))))
		(property "Footprint" "Oscillator:Oscillator_DIP-14" (at 0 -10.16 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "Oscillatore a cristallo attivo 24.576 MHz, DIP-14 metal can 4-pin" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "XO_24M576_DIP14_0_1"
			(rectangle (start -10.16 5.08) (end 10.16 -5.08) (stroke (width 0.254) (type default)) (fill (type background)))
			(text "XO 24.576 MHz" (at 0 1.27 0) (effects (font (size 1.27 1.27) (bold yes))))
			(text "DIP-14 (Metal Can)" (at 0 -1.27 0) (effects (font (size 1.016 1.016))))
		)
		(symbol "XO_24M576_DIP14_1_1"
			(pin input line (at -15.24 2.54 0) (length 5.08) (name "OE" (effects (font (size 1.27 1.27)))) (number "1" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at -15.24 -2.54 0) (length 5.08) (name "GND" (effects (font (size 1.27 1.27)))) (number "4" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at 15.24 2.54 180) (length 5.08) (name "VCC" (effects (font (size 1.27 1.27)))) (number "8" (effects (font (size 1.27 1.27)))))
			(pin output line (at 15.24 -2.54 180) (length 5.08) (name "OUT" (effects (font (size 1.27 1.27)))) (number "5" (effects (font (size 1.27 1.27)))))
		)
	)
	(symbol "PCM1808_Breakout_C"
		(pin_names (offset 1.016))
		(in_bom yes)
		(on_board yes)
		(property "Reference" "MOD" (at 0 20.32 0) (effects (font (size 1.27 1.27))))
		(property "Value" "PCM1808_Breakout_C" (at 0 -22.86 0) (effects (font (size 1.27 1.27))))
		(property "Footprint" "" (at 0 -25.4 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "https://www.ti.com/lit/ds/symlink/pcm1808.pdf" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "Breakout stereo ADC PCM1808 (modulo C, Master 96 kHz) - TI SLES177B (Rev. Aug 2015)" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "PCM1808_Breakout_C_0_1"
			(rectangle (start -17.78 17.78) (end 17.78 -17.78) (stroke (width 0.254) (type default)) (fill (type background)))
			(text "PCM1808 Breakout C" (at 0 11.43 0) (effects (font (size 1.27 1.27) (bold yes))))
			(text "Modulo ADC (TI PCM1808)" (at 0 8.89 0) (effects (font (size 1.016 1.016))))
			(text "Master Mode 96 kHz" (at 0 6.35 0) (effects (font (size 1.016 1.016) (bold yes))))
			(text "TSSOP-14 (SLES177B Rev. Aug 2015)" (at 0 3.81 0) (effects (font (size 0.762 0.762) (italic yes))))
			(text "Pista interna 3.3V (J2.1<->J1.2)" (at 0 0.5 0) (effects (font (size 0.762 0.762) (bold yes))))
			(text "Presunta su C (0R su #1/#2 in F0)" (at 0 -1.8 0) (effects (font (size 0.635 0.635) (italic yes))))
			(text "Piano GND (AGND pin 2 <-> GND 2)" (at 0 -4.2 0) (effects (font (size 0.762 0.762) (bold yes))))
			(text "Misurato ~0,2R su C (altre presunte)" (at 0 -6.5 0) (effects (font (size 0.635 0.635) (italic yes))))
			(text "SCK (J2.3) -> R~100R -> SCKI (Pin 6)" (at 0 -9.5 0) (effects (font (size 0.762 0.762))))
		)
		(symbol "PCM1808_Breakout_C_1_1"
			(pin power_in line (at -22.86 12.7 0) (length 5.08) (name "+5V" (effects (font (size 1.27 1.27)))) (number "J1.1" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at -22.86 7.62 0) (length 5.08) (name "3.3" (effects (font (size 1.27 1.27)))) (number "J1.2" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at -22.86 2.54 0) (length 5.08) (name "GND" (effects (font (size 1.27 1.27)))) (number "J1.3" (effects (font (size 1.27 1.27)))))
			(pin input line (at -22.86 -2.54 0) (length 5.08) (name "MD0" (effects (font (size 1.27 1.27)))) (number "J1.4" (effects (font (size 1.27 1.27)))))
			(pin input line (at -22.86 -7.62 0) (length 5.08) (name "MD1" (effects (font (size 1.27 1.27)))) (number "J1.5" (effects (font (size 1.27 1.27)))))
			(pin input line (at -22.86 -12.7 0) (length 5.08) (name "FMT" (effects (font (size 1.27 1.27)))) (number "J1.6" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at 22.86 12.7 180) (length 5.08) (name "3.3V" (effects (font (size 1.27 1.27)))) (number "J2.1" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at 22.86 7.62 180) (length 5.08) (name "GND_2" (effects (font (size 1.27 1.27)))) (number "J2.2" (effects (font (size 1.27 1.27)))))
			(pin input line (at 22.86 2.54 180) (length 5.08) (name "SCK" (effects (font (size 1.27 1.27)))) (number "J2.3" (effects (font (size 1.27 1.27)))))
			(pin output line (at 22.86 -2.54 180) (length 5.08) (name "LRC" (effects (font (size 1.27 1.27)))) (number "J2.4" (effects (font (size 1.27 1.27)))))
			(pin output line (at 22.86 -7.62 180) (length 5.08) (name "OUT" (effects (font (size 1.27 1.27)))) (number "J2.5" (effects (font (size 1.27 1.27)))))
			(pin output line (at 22.86 -12.7 180) (length 5.08) (name "BCK" (effects (font (size 1.27 1.27)))) (number "J2.6" (effects (font (size 1.27 1.27)))))
			(pin input line (at -10.16 -22.86 90) (length 5.08) (name "RIN" (effects (font (size 1.27 1.27)))) (number "J3.1" (effects (font (size 1.27 1.27)))))
			(pin power_in line (at 0 -22.86 90) (length 5.08) (name "GND" (effects (font (size 1.27 1.27)))) (number "J3.2" (effects (font (size 1.27 1.27)))))
			(pin input line (at 10.16 -22.86 90) (length 5.08) (name "LIN" (effects (font (size 1.27 1.27)))) (number "J3.3" (effects (font (size 1.27 1.27)))))
		)
	)
	(symbol "Logic_Analyzer_8CH"
		(pin_names (offset 1.016))
		(in_bom no)
		(on_board no)
		(property "Reference" "INSTR" (at 0 17.78 0) (effects (font (size 1.27 1.27))))
		(property "Value" "Logic_Analyzer_8CH" (at 0 -17.78 0) (effects (font (size 1.27 1.27))))
		(property "Footprint" "" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "Analizzatore logico USB 8CH / 24 MHz (PulseView) - Strumento passivo esterno (escluso da netlist)" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "Logic_Analyzer_8CH_0_1"
			(rectangle (start -15.24 15.24) (end 15.24 -15.24) (stroke (width 0.254) (type default)) (fill (type background)))
			(text "Logic Analyzer" (at 0 10.16 0) (effects (font (size 1.27 1.27) (bold yes))))
			(text "8CH / 24 MHz" (at 0 7.62 0) (effects (font (size 1.016 1.016))))
			(text "PulseView / sigrok" (at 0 5.08 0) (effects (font (size 1.016 1.016) (italic yes))))
			(text "STRUMENTO PASSIVO ESTERNO" (at 0 1.27 0) (effects (font (size 0.762 0.762) (bold yes))))
			(text "Non alimenta il circuito" (at 0 -1.27 0) (effects (font (size 0.762 0.762))))
			(text "Escluso da netlist PCB (on_board no)" (at 0 -3.81 0) (effects (font (size 0.762 0.762) (italic yes))))
			(text "SCK 24MHz NON connesso" (at 0 -7.62 0) (effects (font (size 0.889 0.889) (bold yes))))
		)
		(symbol "Logic_Analyzer_8CH_1_1"
			(pin power_in line (at -20.32 10.16 0) (length 5.08) (name "GND" (effects (font (size 1.27 1.27)))) (number "1" (effects (font (size 1.27 1.27)))))
			(pin input line (at -20.32 5.08 0) (length 5.08) (name "CH1_D0" (effects (font (size 1.27 1.27)))) (number "2" (effects (font (size 1.27 1.27)))))
			(pin input line (at -20.32 0 0) (length 5.08) (name "CH2_D1" (effects (font (size 1.27 1.27)))) (number "3" (effects (font (size 1.27 1.27)))))
			(pin input line (at -20.32 -5.08 0) (length 5.08) (name "CH3_D2" (effects (font (size 1.27 1.27)))) (number "4" (effects (font (size 1.27 1.27)))))
			(pin input line (at -20.32 -10.16 0) (length 5.08) (name "CH4_D3" (effects (font (size 1.27 1.27)))) (number "5" (effects (font (size 1.27 1.27)))))
			(pin input line (at 20.32 5.08 180) (length 5.08) (name "CH5_D4" (effects (font (size 1.27 1.27)))) (number "6" (effects (font (size 1.27 1.27)))))
			(pin input line (at 20.32 0 180) (length 5.08) (name "CH6_D5" (effects (font (size 1.27 1.27)))) (number "7" (effects (font (size 1.27 1.27)))))
			(pin input line (at 20.32 -5.08 180) (length 5.08) (name "CH7_D6" (effects (font (size 1.27 1.27)))) (number "8" (effects (font (size 1.27 1.27)))))
			(pin input line (at 20.32 -10.16 180) (length 5.08) (name "CH8_D7" (effects (font (size 1.27 1.27)))) (number "9" (effects (font (size 1.27 1.27)))))
		)
	)
	(symbol "R"
		(pin_numbers (hide yes))
		(pin_names (offset 0))
		(in_bom yes)
		(on_board yes)
		(property "Reference" "R" (at 2.032 0 90) (effects (font (size 1.27 1.27))))
		(property "Value" "R" (at 0 0 90) (effects (font (size 1.27 1.27))))
		(property "Footprint" "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "Resistor" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "R_0_1"
			(rectangle (start -1.016 2.54) (end 1.016 -2.54) (stroke (width 0.254) (type default)) (fill (type none)))
		)
		(symbol "R_1_1"
			(pin passive line (at 0 5.08 270) (length 2.54) (name "~" (effects (font (size 1.27 1.27)))) (number "1" (effects (font (size 1.27 1.27)))))
			(pin passive line (at 0 -5.08 90) (length 2.54) (name "~" (effects (font (size 1.27 1.27)))) (number "2" (effects (font (size 1.27 1.27)))))
		)
	)
	(symbol "C"
		(pin_numbers (hide yes))
		(pin_names (offset 0))
		(in_bom yes)
		(on_board yes)
		(property "Reference" "C" (at 2.032 0 90) (effects (font (size 1.27 1.27))))
		(property "Value" "C" (at 0 0 90) (effects (font (size 1.27 1.27))))
		(property "Footprint" "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Datasheet" "" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(property "Description" "Condensatore di bypass locale 100 nF per XO (documentato in FASE 3, non nuovamente verificato su C)" (at 0 0 0) (effects (font (size 1.27 1.27)) (hide yes)))
		(symbol "C_0_1"
			(polyline (pts (xy -2.032 0.762) (xy 2.032 0.762)) (stroke (width 0.381) (type default)) (fill (type none)))
			(polyline (pts (xy -2.032 -0.762) (xy 2.032 -0.762)) (stroke (width 0.381) (type default)) (fill (type none)))
		)
		(symbol "C_1_1"
			(pin passive line (at 0 3.81 270) (length 3.048) (name "~" (effects (font (size 1.27 1.27)))) (number "1" (effects (font (size 1.27 1.27)))))
			(pin passive line (at 0 -3.81 90) (length 3.048) (name "~" (effects (font (size 1.27 1.27)))) (number "2" (effects (font (size 1.27 1.27)))))
		)
	)
)
"""

with open(os.path.join(OUTPUT_DIR, "tickform_symbols.kicad_sym"), "w", encoding="utf-8") as f:
    f.write(sym_lib)

# 4. Generate schematic
SCH_UUID = "d480a719-75c1-4822-b91c-196d4b29a001"

# Prepare symbol instances with deterministic UUIDs
u_uuid = {
    "U1": det_uuid("U1"), "U1_p1": det_uuid("U1_p1"), "U1_p2": det_uuid("U1_p2"), "U1_p3": det_uuid("U1_p3"), "U1_p4": det_uuid("U1_p4"),
    "U2": det_uuid("U2"), "U2_p1": det_uuid("U2_p1"), "U2_p4": det_uuid("U2_p4"), "U2_p5": det_uuid("U2_p5"), "U2_p8": det_uuid("U2_p8"),
    "C1": det_uuid("C1"), "C1_p1": det_uuid("C1_p1"), "C1_p2": det_uuid("C1_p2"),
    "MOD1": det_uuid("MOD1"),
    "MOD1_p1": det_uuid("MOD1_p1"), "MOD1_p2": det_uuid("MOD1_p2"), "MOD1_p3": det_uuid("MOD1_p3"), "MOD1_p4": det_uuid("MOD1_p4"),
    "MOD1_p5": det_uuid("MOD1_p5"), "MOD1_p6": det_uuid("MOD1_p6"), "MOD1_p7": det_uuid("MOD1_p7"), "MOD1_p8": det_uuid("MOD1_p8"),
    "MOD1_p9": det_uuid("MOD1_p9"), "MOD1_p10": det_uuid("MOD1_p10"), "MOD1_p11": det_uuid("MOD1_p11"), "MOD1_p12": det_uuid("MOD1_p12"),
    "MOD1_p13": det_uuid("MOD1_p13"), "MOD1_p14": det_uuid("MOD1_p14"), "MOD1_p15": det_uuid("MOD1_p15"),
    "R1": det_uuid("R1"), "R1_p1": det_uuid("R1_p1"), "R1_p2": det_uuid("R1_p2"),
    "R2": det_uuid("R2"), "R2_p1": det_uuid("R2_p1"), "R2_p2": det_uuid("R2_p2"),
    "INSTR1": det_uuid("INSTR1"),
    "INSTR1_p1": det_uuid("INSTR1_p1"), "INSTR1_p2": det_uuid("INSTR1_p2"), "INSTR1_p3": det_uuid("INSTR1_p3"), "INSTR1_p4": det_uuid("INSTR1_p4"),
    "INSTR1_p5": det_uuid("INSTR1_p5"), "INSTR1_p6": det_uuid("INSTR1_p6"), "INSTR1_p7": det_uuid("INSTR1_p7"), "INSTR1_p8": det_uuid("INSTR1_p8"),
    "INSTR1_p9": det_uuid("INSTR1_p9"),
}

# Embedded lib_symbols block (matches tickform_symbols.kicad_sym with library prefix)
sym_raw_lines = sym_lib.splitlines()
lib_sym_lines = ['\t(lib_symbols']
for line in sym_raw_lines[4:-1]:
    if line.startswith('\t(symbol "'):
        sname = line.split('"')[1]
        lib_sym_lines.append(f'\t\t(symbol "tickform_symbols:{sname}"')
    else:
        lib_sym_lines.append('\t' + line)
lib_sym_lines.append('\t)')
embedded_lib_symbols = '\n'.join(lib_sym_lines)

# Full kicad_sch content
sch_lines = []
sch_lines.append('(kicad_sch')
sch_lines.append('\t(version 20250114)')
sch_lines.append('\t(generator "eeschema")')
sch_lines.append('\t(generator_version "10.0")')
sch_lines.append(f'\t(uuid "{SCH_UUID}")')
sch_lines.append('\t(paper "A3")')
sch_lines.append('\t(title_block')
sch_lines.append('\t\t(title "TICKFORM — FASE 4 — PCM1808 C VERIFICA LRCK 96 kHz")')
sch_lines.append('\t\t(date "2026-11-13")')
sch_lines.append('\t\t(rev "R0.1")')
sch_lines.append('\t\t(company "Tickform Open Source Timegrapher")')
sch_lines.append('\t\t(comment 1 "Configurazione banco collaudo PCM1808 C: LRCK 96 kHz / BCK ~6.144 MHz")')
sch_lines.append('\t\t(comment 2 "WeAct STM32G431 + XO 24.576 MHz + Breakout C + Logic Analyzer 8CH (Rev. R0.1)")')
sch_lines.append('\t)')
sch_lines.append(embedded_lib_symbols)

# Helpers with deterministic UUIDs
def add_junction(x, y):
    sch_lines.append('\t(junction')
    sch_lines.append(f'\t\t(at {x:.2f} {y:.2f})')
    sch_lines.append('\t\t(diameter 1.016)')
    sch_lines.append('\t\t(color 0 0 0 0)')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"junc_{x:.2f}_{y:.2f}")}")')
    sch_lines.append('\t)')

def add_wire(x1, y1, x2, y2):
    sch_lines.append('\t(wire')
    sch_lines.append('\t\t(pts')
    sch_lines.append(f'\t\t\t(xy {x1:.2f} {y1:.2f}) (xy {x2:.2f} {y2:.2f})')
    sch_lines.append('\t\t)')
    sch_lines.append('\t\t(stroke')
    sch_lines.append('\t\t\t(width 0)')
    sch_lines.append('\t\t\t(type solid)')
    sch_lines.append('\t\t)')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"wire_{x1:.2f}_{y1:.2f}_{x2:.2f}_{y2:.2f}")}")')
    sch_lines.append('\t)')

def add_no_connect(x, y):
    sch_lines.append('\t(no_connect')
    sch_lines.append(f'\t\t(at {x:.2f} {y:.2f})')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"nc_{x:.2f}_{y:.2f}")}")')
    sch_lines.append('\t)')

def add_label(text, x, y, rotation=0):
    sch_lines.append(f'\t(label "{text}"')
    sch_lines.append(f'\t\t(at {x:.2f} {y:.2f} {rotation})')
    sch_lines.append('\t\t(effects')
    sch_lines.append('\t\t\t(font')
    sch_lines.append('\t\t\t\t(size 1.27 1.27)')
    sch_lines.append('\t\t\t)')
    sch_lines.append('\t\t\t(justify left bottom)')
    sch_lines.append('\t\t)')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"lbl_{text}_{x:.2f}_{y:.2f}")}")')
    sch_lines.append('\t)')

def add_text(text, x, y, size=1.5, bold="no", italic="no"):
    sch_lines.append(f'\t(text "{text}"')
    sch_lines.append('\t\t(exclude_from_sim no)')
    sch_lines.append(f'\t\t(at {x:.2f} {y:.2f} 0)')
    sch_lines.append('\t\t(effects')
    sch_lines.append('\t\t\t(font')
    sch_lines.append(f'\t\t\t\t(size {size:.2f} {size:.2f})')
    if bold == "yes":
        sch_lines.append('\t\t\t\t(bold yes)')
    if italic == "yes":
        sch_lines.append('\t\t\t\t(italic yes)')
    sch_lines.append('\t\t\t)')
    sch_lines.append('\t\t\t(justify left bottom)')
    sch_lines.append('\t\t)')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"txt_{x:.2f}_{y:.2f}")}")')
    sch_lines.append('\t)')

def add_dashed_box(x1, y1, x2, y2):
    sch_lines.append('\t(polyline')
    sch_lines.append('\t\t(pts')
    sch_lines.append(f'\t\t\t(xy {x1:.2f} {y1:.2f}) (xy {x2:.2f} {y1:.2f}) (xy {x2:.2f} {y2:.2f}) (xy {x1:.2f} {y2:.2f}) (xy {x1:.2f} {y1:.2f})')
    sch_lines.append('\t\t)')
    sch_lines.append('\t\t(stroke')
    sch_lines.append('\t\t\t(width 0.254)')
    sch_lines.append('\t\t\t(type dash)')
    sch_lines.append('\t\t)')
    sch_lines.append('\t\t(fill (type none))')
    sch_lines.append(f'\t\t(uuid "{det_uuid(f"box_{x1:.2f}_{y1:.2f}")}")')
    sch_lines.append('\t)')

# Coordinates:
X_U1, Y_U1 = 60.96, 127.0
X_U2, Y_U2 = 139.7, 76.2
X_C1, Y_C1 = 170.18, 76.2
X_MOD, Y_MOD = 236.22, 127.0
X_R1, Y_R1 = 198.12, 124.46
X_R2, Y_R2 = 187.96, 127.0
X_INSTR, Y_INSTR = 345.44, 127.0

# --- Add Symbol Instances ---

# U1: WeAct
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:WeAct_STM32G431_Header")')
sch_lines.append(f'\t\t(at {X_U1:.2f} {Y_U1:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["U1"]}")')
sch_lines.append('\t\t(property "Reference" "U1"')
sch_lines.append(f'\t\t\t(at {X_U1:.2f} {Y_U1-10.16:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "WeAct_STM32G431_Header"')
sch_lines.append(f'\t\t\t(at {X_U1:.2f} {Y_U1+10.16:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical"')
sch_lines.append(f'\t\t\t(at {X_U1:.2f} {Y_U1+12.7:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" "https://github.com/WeActStudio/WeActStudio.STM32G431CoreBoard"')
sch_lines.append(f'\t\t\t(at {X_U1:.2f} {Y_U1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "WeAct Studio V1.0 STM32G431 Core Board - Header destro (pin 1..4)"')
sch_lines.append(f'\t\t\t(at {X_U1:.2f} {Y_U1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["U1_p1"]}"))')
sch_lines.append(f'\t\t(pin "2" (uuid "{u_uuid["U1_p2"]}"))')
sch_lines.append(f'\t\t(pin "3" (uuid "{u_uuid["U1_p3"]}"))')
sch_lines.append(f'\t\t(pin "4" (uuid "{u_uuid["U1_p4"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "U1") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# U2: XO 24.576 MHz
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:XO_24M576_DIP14")')
sch_lines.append(f'\t\t(at {X_U2:.2f} {Y_U2:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["U2"]}")')
sch_lines.append('\t\t(property "Reference" "U2"')
sch_lines.append(f'\t\t\t(at {X_U2:.2f} {Y_U2-7.62:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "XO_24M576_DIP14"')
sch_lines.append(f'\t\t\t(at {X_U2:.2f} {Y_U2+7.62:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" "Oscillator:Oscillator_DIP-14"')
sch_lines.append(f'\t\t\t(at {X_U2:.2f} {Y_U2+10.16:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" ""')
sch_lines.append(f'\t\t\t(at {X_U2:.2f} {Y_U2:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Oscillatore a cristallo attivo 24.576 MHz, DIP-14 metal can 4-pin"')
sch_lines.append(f'\t\t\t(at {X_U2:.2f} {Y_U2:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["U2_p1"]}"))')
sch_lines.append(f'\t\t(pin "4" (uuid "{u_uuid["U2_p4"]}"))')
sch_lines.append(f'\t\t(pin "8" (uuid "{u_uuid["U2_p8"]}"))')
sch_lines.append(f'\t\t(pin "5" (uuid "{u_uuid["U2_p5"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "U2") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# C1: Bypass Capacitor (XO)
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:C")')
sch_lines.append(f'\t\t(at {X_C1:.2f} {Y_C1:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["C1"]}")')
sch_lines.append('\t\t(property "Reference" "C1"')
sch_lines.append(f'\t\t\t(at {X_C1+3.81:.2f} {Y_C1-1.27:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (justify left))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "100nF"')
sch_lines.append(f'\t\t\t(at {X_C1+3.81:.2f} {Y_C1+1.27:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (justify left))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm"')
sch_lines.append(f'\t\t\t(at {X_C1:.2f} {Y_C1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" ""')
sch_lines.append(f'\t\t\t(at {X_C1:.2f} {Y_C1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Condensatore di bypass locale 100 nF per XO (documentato in FASE 3, non nuovamente verificato su C)"')
sch_lines.append(f'\t\t\t(at {X_C1:.2f} {Y_C1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["C1_p1"]}"))')
sch_lines.append(f'\t\t(pin "2" (uuid "{u_uuid["C1_p2"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "C1") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# MOD1: PCM1808 Breakout
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:PCM1808_Breakout_C")')
sch_lines.append(f'\t\t(at {X_MOD:.2f} {Y_MOD:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["MOD1"]}")')
sch_lines.append('\t\t(property "Reference" "MOD1"')
sch_lines.append(f'\t\t\t(at {X_MOD:.2f} {Y_MOD-20.32:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "PCM1808_Breakout_C"')
sch_lines.append(f'\t\t\t(at {X_MOD:.2f} {Y_MOD+20.32:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" ""')
sch_lines.append(f'\t\t\t(at {X_MOD:.2f} {Y_MOD+22.86:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" "https://www.ti.com/lit/ds/symlink/pcm1808.pdf"')
sch_lines.append(f'\t\t\t(at {X_MOD:.2f} {Y_MOD:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Breakout stereo ADC PCM1808 (modulo C, Master 96 kHz) - TI SLES177B (Rev. Aug 2015)"')
sch_lines.append(f'\t\t\t(at {X_MOD:.2f} {Y_MOD:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "J1.1" (uuid "{u_uuid["MOD1_p1"]}"))')
sch_lines.append(f'\t\t(pin "J1.2" (uuid "{u_uuid["MOD1_p2"]}"))')
sch_lines.append(f'\t\t(pin "J1.3" (uuid "{u_uuid["MOD1_p3"]}"))')
sch_lines.append(f'\t\t(pin "J1.4" (uuid "{u_uuid["MOD1_p4"]}"))')
sch_lines.append(f'\t\t(pin "J1.5" (uuid "{u_uuid["MOD1_p5"]}"))')
sch_lines.append(f'\t\t(pin "J1.6" (uuid "{u_uuid["MOD1_p6"]}"))')
sch_lines.append(f'\t\t(pin "J2.1" (uuid "{u_uuid["MOD1_p7"]}"))')
sch_lines.append(f'\t\t(pin "J2.2" (uuid "{u_uuid["MOD1_p8"]}"))')
sch_lines.append(f'\t\t(pin "J2.3" (uuid "{u_uuid["MOD1_p9"]}"))')
sch_lines.append(f'\t\t(pin "J2.4" (uuid "{u_uuid["MOD1_p10"]}"))')
sch_lines.append(f'\t\t(pin "J2.5" (uuid "{u_uuid["MOD1_p11"]}"))')
sch_lines.append(f'\t\t(pin "J2.6" (uuid "{u_uuid["MOD1_p12"]}"))')
sch_lines.append(f'\t\t(pin "J3.1" (uuid "{u_uuid["MOD1_p13"]}"))')
sch_lines.append(f'\t\t(pin "J3.2" (uuid "{u_uuid["MOD1_p14"]}"))')
sch_lines.append(f'\t\t(pin "J3.3" (uuid "{u_uuid["MOD1_p15"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "MOD1") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# R1: 10k pullup for MD0
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:R")')
sch_lines.append(f'\t\t(at {X_R1:.2f} {Y_R1:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["R1"]}")')
sch_lines.append('\t\t(property "Reference" "R1"')
sch_lines.append(f'\t\t\t(at {X_R1-2.54:.2f} {Y_R1:.2f} 90)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "10k"')
sch_lines.append(f'\t\t\t(at {X_R1+2.54:.2f} {Y_R1:.2f} 90)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal"')
sch_lines.append(f'\t\t\t(at {X_R1:.2f} {Y_R1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" ""')
sch_lines.append(f'\t\t\t(at {X_R1:.2f} {Y_R1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Resistenza di pull-up 10 kΩ 1% per MD0"')
sch_lines.append(f'\t\t\t(at {X_R1:.2f} {Y_R1:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["R1_p1"]}"))')
sch_lines.append(f'\t\t(pin "2" (uuid "{u_uuid["R1_p2"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "R1") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# R2: 10k pullup for MD1
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:R")')
sch_lines.append(f'\t\t(at {X_R2:.2f} {Y_R2:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom yes)')
sch_lines.append('\t\t(on_board yes)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["R2"]}")')
sch_lines.append('\t\t(property "Reference" "R2"')
sch_lines.append(f'\t\t\t(at {X_R2-2.54:.2f} {Y_R2:.2f} 90)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "10k"')
sch_lines.append(f'\t\t\t(at {X_R2+2.54:.2f} {Y_R2:.2f} 90)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal"')
sch_lines.append(f'\t\t\t(at {X_R2:.2f} {Y_R2:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" ""')
sch_lines.append(f'\t\t\t(at {X_R2:.2f} {Y_R2:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Resistenza di pull-up 10 kΩ 1% per MD1"')
sch_lines.append(f'\t\t\t(at {X_R2:.2f} {Y_R2:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["R2_p1"]}"))')
sch_lines.append(f'\t\t(pin "2" (uuid "{u_uuid["R2_p2"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "R2") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# INSTR1: Logic Analyzer
sch_lines.append('\t(symbol')
sch_lines.append('\t\t(lib_id "tickform_symbols:Logic_Analyzer_8CH")')
sch_lines.append(f'\t\t(at {X_INSTR:.2f} {Y_INSTR:.2f} 0)')
sch_lines.append('\t\t(unit 1)')
sch_lines.append('\t\t(exclude_from_sim no)')
sch_lines.append('\t\t(in_bom no)')
sch_lines.append('\t\t(on_board no)')
sch_lines.append('\t\t(dnp no)')
sch_lines.append(f'\t\t(uuid "{u_uuid["INSTR1"]}")')
sch_lines.append('\t\t(property "Reference" "INSTR1"')
sch_lines.append(f'\t\t\t(at {X_INSTR:.2f} {Y_INSTR-17.78:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Value" "Logic_Analyzer_8CH"')
sch_lines.append(f'\t\t\t(at {X_INSTR:.2f} {Y_INSTR+17.78:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Footprint" ""')
sch_lines.append(f'\t\t\t(at {X_INSTR:.2f} {Y_INSTR:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Datasheet" ""')
sch_lines.append(f'\t\t\t(at {X_INSTR:.2f} {Y_INSTR:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append('\t\t(property "Description" "Analizzatore logico USB 8CH / 24 MHz (PulseView) - Strumento passivo esterno (escluso da netlist)"')
sch_lines.append(f'\t\t\t(at {X_INSTR:.2f} {Y_INSTR:.2f} 0)')
sch_lines.append('\t\t\t(effects (font (size 1.27 1.27)) (hide yes))')
sch_lines.append('\t\t)')
sch_lines.append(f'\t\t(pin "1" (uuid "{u_uuid["INSTR1_p1"]}"))')
sch_lines.append(f'\t\t(pin "2" (uuid "{u_uuid["INSTR1_p2"]}"))')
sch_lines.append(f'\t\t(pin "3" (uuid "{u_uuid["INSTR1_p3"]}"))')
sch_lines.append(f'\t\t(pin "4" (uuid "{u_uuid["INSTR1_p4"]}"))')
sch_lines.append(f'\t\t(pin "5" (uuid "{u_uuid["INSTR1_p5"]}"))')
sch_lines.append(f'\t\t(pin "6" (uuid "{u_uuid["INSTR1_p6"]}"))')
sch_lines.append(f'\t\t(pin "7" (uuid "{u_uuid["INSTR1_p7"]}"))')
sch_lines.append(f'\t\t(pin "8" (uuid "{u_uuid["INSTR1_p8"]}"))')
sch_lines.append(f'\t\t(pin "9" (uuid "{u_uuid["INSTR1_p9"]}"))')
sch_lines.append('\t\t(instances')
sch_lines.append('\t\t\t(project "tickform_phase4_pcm1808c"')
sch_lines.append(f'\t\t\t\t(path "/{SCH_UUID}" (reference "INSTR1") (unit 1))')
sch_lines.append('\t\t\t)')
sch_lines.append('\t\t)')
sch_lines.append('\t)')

# --- No Connect Markers ---
add_no_connect(73.66, 128.27)    # U1 Pin 3 (B11)
add_no_connect(124.46, 73.66)   # U2 Pin 1 (OE)
add_no_connect(213.36, 124.46)  # MOD1 Pin J1.3 (GND - NC esterno; massa di alimentazione su J2.2)
add_no_connect(213.36, 139.70)  # MOD1 Pin J1.6 (FMT)
add_no_connect(259.08, 134.62)  # MOD1 Pin J2.5 (OUT)
add_no_connect(226.06, 149.86)  # MOD1 Pin J3.1 (RIN)
add_no_connect(236.22, 149.86)  # MOD1 Pin J3.2 (GND analog)
add_no_connect(246.38, 149.86)  # MOD1 Pin J3.3 (LIN)
add_no_connect(325.12, 132.08)  # INSTR1 Pin 4 (CH3_D2)
add_no_connect(325.12, 137.16)  # INSTR1 Pin 5 (CH4_D3)
add_no_connect(365.76, 121.92)  # INSTR1 Pin 6 (CH5_D4)
add_no_connect(365.76, 127.00)  # INSTR1 Pin 7 (CH6_D5)
add_no_connect(365.76, 132.08)  # INSTR1 Pin 8 (CH7_D6)
add_no_connect(365.76, 137.16)  # INSTR1 Pin 9 (CH8_D7)

# --- WIRES AND JUNCTIONS ---

# NET +5V: WeAct V+ (73.66, 130.81) to MOD1 J1.1 (+5V) (213.36, 114.30)
# CAVETTO ESTERNO DIRETTO
add_wire(73.66, 130.81, 81.28, 130.81)
add_wire(81.28, 130.81, 81.28, 160.02)
add_wire(81.28, 160.02, 165.10, 160.02)
add_wire(165.10, 160.02, 165.10, 114.30)
add_wire(165.10, 114.30, 213.36, 114.30)
add_label("+5V", 90.0, 160.02, 0)

# NET 3V3: WeAct Pin 1 (73.66, 123.19)
# -> XO Pin 8 (154.94, 73.66) [CAVETTO ESTERNO]
# -> C1 Pin 1 (170.18, 72.39) [Bypass XO documentato FASE 3]
# -> MOD1 J2.1 (259.08, 114.30) [CAVETTO ESTERNO DI ALIMENTAZIONE DIGITALE]
add_wire(73.66, 123.19, 93.98, 123.19)
add_wire(93.98, 123.19, 93.98, 66.04)
add_wire(93.98, 66.04, 154.94, 66.04)
add_junction(154.94, 66.04)
add_wire(154.94, 66.04, 154.94, 73.66) # To XO Pin 8
add_wire(154.94, 66.04, 170.18, 66.04)
add_junction(170.18, 66.04)
add_wire(170.18, 66.04, 170.18, 72.39) # To C1 Pin 1
add_wire(170.18, 66.04, 259.08, 66.04)
add_wire(259.08, 66.04, 259.08, 114.30) # To MOD1 J2.1 (3.3V)
add_label("3V3", 100.0, 66.04, 0)

# NET GND: WeAct Pin 2 (73.66, 125.73)
# -> XO Pin 4 (124.46, 78.74) [CAVETTO ESTERNO]
# -> C1 Pin 2 (170.18, 80.01) [Bypass XO]
# -> MOD1 J2.2 GND_2 (259.08, 119.38) [CAVETTO ESTERNO MASSA MODULO]
# -> INSTR1 Pin 1 (325.12, 116.84) [CAVETTO ESTERNO MASSA STRUMENTO]
# NOTA: J1.3 (GND) NON ha cavo esterno (è contrassegnato con No-Connect; piano interno al modulo)
add_wire(73.66, 125.73, 106.68, 125.73)
add_wire(106.68, 125.73, 106.68, 86.36)
add_wire(106.68, 86.36, 116.84, 86.36)
add_junction(116.84, 86.36)
add_wire(116.84, 86.36, 116.84, 78.74)
add_wire(116.84, 78.74, 124.46, 78.74) # To XO Pin 4
add_wire(116.84, 86.36, 170.18, 86.36)
add_junction(170.18, 86.36)
add_wire(170.18, 86.36, 170.18, 80.01) # To C1 Pin 2
add_wire(170.18, 86.36, 266.70, 86.36)
add_wire(266.70, 86.36, 266.70, 116.84)
add_junction(266.70, 116.84)
add_wire(266.70, 116.84, 325.12, 116.84) # To INSTR1 Pin 1 GND
add_wire(266.70, 116.84, 266.70, 119.38)
add_wire(266.70, 119.38, 259.08, 119.38) # To MOD1 J2.2 GND_2
add_label("GND", 130.0, 86.36, 0)

# Breakout 3.3V Distribution to Pull-ups:
# Sul banco fisico, R1 e R2 sono inseriti nel terminale J1.2 ("3.3").
# Non c'è alcun cavo esterno tra J2.1 e J1.2: la tensione arriva a J1.2 via pista interna al modulo.
# Nello schema CAD, J1.2 alimenta R1 e R2 sulla net 3V3 (continuità interna presunta su C).
add_wire(213.36, 119.38, 198.12, 119.38) # J1.2 connects to R1 Pin 1
add_junction(198.12, 119.38)
add_wire(198.12, 119.38, 187.96, 119.38)
add_wire(187.96, 119.38, 187.96, 121.92) # connects to R2 Pin 1
add_label("3V3", 200.0, 119.38, 0)

# R1 Pin 2 (198.12, 129.54) -> MOD1 J1.4 MD0 (213.36, 129.54)
add_wire(198.12, 129.54, 213.36, 129.54)
add_label("MD0", 205.0, 129.54, 0)

# R2 Pin 2 (187.96, 132.08) -> MOD1 J1.5 MD1 (213.36, 134.62)
add_wire(187.96, 132.08, 187.96, 134.62)
add_wire(187.96, 134.62, 213.36, 134.62)
add_label("MD1", 205.0, 134.62, 0)

# SCK Direct Wire: XO Pin 5 OUT (154.94, 78.74) to MOD1 J2.3 SCK (259.08, 124.46)
# CAVETTO ESTERNO DIRETTO (nessuna resistenza esterna)
add_wire(154.94, 78.74, 274.32, 78.74)
add_wire(274.32, 78.74, 274.32, 124.46)
add_wire(274.32, 124.46, 259.08, 124.46)
add_label("PCM_SCK", 210.0, 78.74, 0)

# LRC Wire: MOD1 J2.4 LRC (259.08, 129.54) to INSTR1 CH1_D0 (325.12, 121.92)
# SONDA LOGIC ANALYZER
add_wire(259.08, 129.54, 284.48, 129.54)
add_wire(284.48, 129.54, 284.48, 121.92)
add_wire(284.48, 121.92, 325.12, 121.92)
add_label("PCM_LRC", 295.0, 121.92, 0)

# BCK Wire: MOD1 J2.6 BCK (259.08, 139.70) to INSTR1 CH2_D1 (325.12, 127.00)
# SONDA LOGIC ANALYZER
add_wire(259.08, 139.70, 297.18, 139.70)
add_wire(297.18, 139.70, 297.18, 127.00)
add_wire(297.18, 127.00, 325.12, 127.00)
add_label("PCM_BCK", 305.0, 127.00, 0)

# Dashed box around Logic Analyzer
add_dashed_box(312.42, 99.06, 378.46, 154.94)

# Text Callouts and Notes on Schematic Canvas
add_text("STRUMENTAZIONE DI MISURA ESTERNA (PULSEVIEW)", 314.96, 103.0, size=1.5, bold="yes")
add_text("Analizzatore logico USB 8CH / 24 MHz - Solo acquisizione passiva", 314.96, 107.0, size=1.1, italic="yes")
add_text("AVVISO: Escluso da netlist PCB (on_board no / in_bom no)", 314.96, 111.0, size=0.9, italic="yes")
add_text("AVVISO: SCK (24.576 MHz) NON connesso (limite di banda 24 MS/s)", 314.96, 150.0, size=1.1, bold="yes")

# Local Pin Notes on Schematic
add_text("J1.3 (GND): NC esterno (piano GND interno presunto su C)", 148.0, 123.5, size=0.9, italic="yes")
add_text("J1.2 (3.3): Distribuzione R1/R2 (pista interna da J2.1 presunta su C)", 135.0, 116.5, size=0.9, italic="yes")
add_text("C1 100nF: Documentato in FASE 3, non nuovamente verificato", 145.0, 92.0, size=0.9, italic="yes")

# Note Box 1: Experimental Evidence
add_text("1. EVIDENZA SPERIMENTALE COLLAUDO PCM1808 C (12/11/2026):", 30.48, 178.0, size=1.5, bold="yes")
add_text("- Tensione VCC analogica misurata: 5,0 V (rail WeAct V+ da bus USB-C del PC).", 30.48, 182.5, size=1.1)
add_text("- Tensione VDD digitale misurata: 3,2 V (rail WeAct 3V3 da regolatore LDO onboard).", 30.48, 186.5, size=1.1)
add_text("- Frequenza LRCK: 96,000 kHz rilevati tramite cursori PulseView.", 30.48, 190.5, size=1.1)
add_text("- Frequenza BCK: circa 16 periodi in 2,625 us su PulseView, compatibile", 30.48, 194.5, size=1.1)
add_text("  con 6,144 MHz nominali (nessuna misura metrologica esatta a 6,144 MHz dichiarata).", 30.48, 198.5, size=1.1)
add_text("- Master clock SCK: 24,576 MHz nominali da oscillatore XO esterno (256 x Fs).", 30.48, 202.5, size=1.1)
add_text("- Firmware MCU STM32G431: Nessuna modifica applicativa (Gate 4 standalone).", 30.48, 206.5, size=1.1)
add_text("- Conversione A/D audio: NON ancora verificata (RIN/LIN aperti, AFE non connesso).", 30.48, 210.5, size=1.1)

# Note Box 2: Clock & Breakout Resistance & Bypass XO
add_text("2. CLOCK SCK, RESISTENZE BREAKOUT E BYPASS XO:", 30.48, 218.0, size=1.5, bold="yes")
add_text("- Collegamento XO OUT -> PCM C SCK realizzato mediante FILO DIRETTO.", 30.48, 222.5, size=1.1)
add_text("- Nessuna resistenza esterna da 100 ohm interposta (a differenza di FASE 3).", 30.48, 226.5, size=1.1)
add_text("- Misura passiva R percorso SCK: circa 100 ohm onboard tra SCK e Pin 6 (SCKI).", 30.48, 230.5, size=1.1)
add_text("- Misura passiva continuita' massa su C: AGND chip Pin 2 -> GND 2 breakout circa 0,2 ohm.", 30.48, 234.5, size=1.1)
add_text("  (Misura confermata su C; non estesa automaticamente ad altre continuita' non misurate).", 30.48, 238.5, size=1.1)
add_text("- Condensatore C1 (100 nF XO bypass): documentato storicamente in FASE 3", 30.48, 242.5, size=1.1)
add_text("  (PROJECT_LOG.md#L180), non nuovamente verificato a banco nel collaudo C.", 30.48, 246.5, size=1.1)

# Note Box 3: Terminal Mapping (TI SLES177B, Rev. Aug 2015)
add_text("3. MAPPATURA TERMINALI BREAKOUT VS PIEDINI CHIP TSSOP-14:", 210.82, 175.0, size=1.5, bold="yes")
add_text("Rif. ufficiale: TI PCM1808 Datasheet SLES177B (APRIL 2006 - REVISED AUGUST 2015):", 210.82, 179.5, size=1.1, italic="yes")
add_text("  * Term. J1.1 (+5V)    -> Chip Pin 3 (VCC Analogica +5V)", 210.82, 183.5, size=1.0)
add_text("  * Term. J1.2 (3.3)    -> Chip Pin 4 (VDD Digitale +3.3V, continuita' interna con J2.1)", 210.82, 187.0, size=1.0)
add_text("  * Term. J1.3 (GND)    -> Chip Pin 2/5 (AGND/DGND massa comune; NC esterno)", 210.82, 190.5, size=1.0)
add_text("  * Term. J1.4 (MD0)    -> Chip Pin 10 (MD0 Mode Control)", 210.82, 194.0, size=1.0)
add_text("  * Term. J1.5 (MD1)    -> Chip Pin 11 (MD1 Mode Control)", 210.82, 197.5, size=1.0)
add_text("  * Term. J1.6 (FMT)    -> Chip Pin 12 (FMT Audio Format)", 210.82, 201.0, size=1.0)
add_text("  * Term. J2.1 (3.3V)   -> Chip Pin 4 (VDD Digitale, alimentazione da WeAct 3V3)", 210.82, 204.5, size=1.0)
add_text("  * Term. J2.2 (GND_2)  -> Chip Pin 2/5 (AGND/DGND, connesso a WeAct GND; ~0,2R a Pin 2)", 210.82, 208.0, size=1.0)
add_text("  * Term. J2.3 (SCK)    -> R_onboard (~100 ohm) -> Chip Pin 6 (SCKI System Clock)", 210.82, 211.5, size=1.0)
add_text("  * Term. J2.4 (LRC)    -> Chip Pin 7 (LRCK Word Clock out, 96 kHz)", 210.82, 215.0, size=1.0)
add_text("  * Term. J2.5 (OUT)    -> Chip Pin 9 (DOUT Audio Data out, non connesso)", 210.82, 218.5, size=1.0)
add_text("  * Term. J2.6 (BCK)    -> Chip Pin 8 (BCK Bit Clock out, ~6.144 MHz)", 210.82, 222.0, size=1.0)
add_text("  * Term. J3.1 (RIN)    -> Chip Pin 14 (VINR Ingresso analogico destro, aperto)", 210.82, 225.5, size=1.0)
add_text("  * Term. J3.2 (GND)    -> Chip Pin 2 (AGND Massa analogica riferimento, aperto)", 210.82, 229.0, size=1.0)
add_text("  * Term. J3.3 (LIN)    -> Chip Pin 13 (VINL Ingresso analogico sinistro, aperto)", 210.82, 232.5, size=1.0)
add_text("  * (Interno silicio)   -> Chip Pin 1 (VREF Tensione riferimento interna)", 210.82, 236.0, size=1.0)

# Note Box 4: Master Mode Config & Power Distribution
add_text("4. CONFIGURAZIONE MASTER E DISTRIBUZIONE 3,3 V (PRECISAZIONE STORICA):", 210.82, 244.0, size=1.5, bold="yes")
add_text("- MD0/MD1 = HIGH (pull-up 10 kohm a 3.3V): Master Mode 256xFs (Fs=96kHz, BCK=64xFs).", 210.82, 248.5, size=1.1)
add_text("- FMT = OPEN: livello LOW garantito da pull-down interno (formato I2S 24 bit).", 210.82, 252.5, size=1.1)
add_text("- DISTINZIONE COLLEGAMENTI ESTERNI VS INTERNI:", 210.82, 256.5, size=1.1, bold="yes")
add_text("  * CAVI ESTERNI REALI: WeAct 3V3 -> J2.1; WeAct GND -> J2.2; WeAct V+ -> J1.1.", 210.82, 260.5, size=1.0)
add_text("    R1 e R2 fisicamente attestati sul terminale J1.2. Nessun cavetto tra J2.1 e J1.2.", 210.82, 264.0, size=1.0)
add_text("  * COLLEGAMENTO INTERNO J2.1 <-> J1.2: PRESUNTO su scheda C; la misura di continuita'", 210.82, 267.5, size=1.0)
add_text("    a 0 ohm e' documentata storicamente SOLO su PCM1808 #1 e #2 in FASE 0 (PROJECT_LOG.md#L88).", 210.82, 271.0, size=1.0)
add_text("  * NOTA STRUMENTAZIONE: Lo strumento INSTR1 compare nello schema ma e' escluso", 210.82, 274.5, size=1.0)
add_text("    dalla netlist PCB (on_board no). I probe LA non sono tracciati come nodi PCB.", 210.82, 278.0, size=1.0)

sch_lines.append('\t(sheet_instances')
sch_lines.append(f'\t\t(path "/" (page "1"))')
sch_lines.append('\t)')
sch_lines.append(')')

sch_content = "\n".join(sch_lines)

with open(os.path.join(OUTPUT_DIR, "tickform_phase4_pcm1808c.kicad_sch"), "w", encoding="utf-8") as f:
    f.write(sch_content)

print("Generated tickform_phase4_pcm1808c.kicad_sch (Rev R0.1 Portable & Deterministic) successfully")
