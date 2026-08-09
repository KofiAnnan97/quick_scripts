package engine

import (
	"fmt"
	"strconv"
	"strings"
	"unicode"
)

type MacroStep struct {
	Inputs  []string `json:"inputs,omitempty"`
	HoldMs  int      `json:"hold_ms,omitempty"`
	Hold    bool     `json:"hold,omitempty"`
	DelayMs int      `json:"delay_ms,omitempty"`
}

type Macro struct {
	Raw   string      `json:"raw"`
	Steps []MacroStep `json:"steps"`
}

func ParseMacro(raw string) (Macro, error) {
	raw = strings.TrimSpace(raw)
	if raw == "" {
		return Macro{}, fmt.Errorf("empty macro input")
	}

	steps := make([]MacroStep, 0)
	for _, token := range tokenize(raw) {
		if token == "" {
			continue
		}

		step, err := parseToken(token)
		if err != nil {
			return Macro{}, err
		}
		if len(step.Inputs) == 0 && step.HoldMs == 0 && step.DelayMs == 0 {
			continue
		}
		steps = append(steps, step)
	}

	if len(steps) == 0 {
		return Macro{}, fmt.Errorf("no valid macro steps found")
	}

	return Macro{
		Raw:   raw,
		Steps: steps,
	}, nil
}

func tokenize(raw string) []string {
	replaced := strings.NewReplacer(
		"\n", " ",
		"\r", " ",
		"\t", " ",
	).Replace(raw)

	return strings.FieldsFunc(replaced, func(r rune) bool {
		return r == ',' || r == ';' || unicode.IsSpace(r)
	})
}

func parseToken(token string) (MacroStep, error) {
	token = strings.TrimSpace(token)
	if token == "" {
		return MacroStep{}, nil
	}

	lower := strings.ToLower(token)
	if strings.HasPrefix(lower, "wait") {
		rest := strings.TrimSpace(strings.TrimPrefix(token, "wait"))
		rest = strings.TrimLeft(rest, ":")
		rest = strings.TrimSpace(rest)

		if rest == "" {
			return MacroStep{DelayMs: 200}, nil
		}

		duration, err := parseDuration(rest)
		if err != nil {
			return MacroStep{}, fmt.Errorf("invalid wait duration %q: %w", rest, err)
		}

		return MacroStep{DelayMs: duration}, nil
	}

	parts := strings.Split(token, "+")
	if len(parts) > 1 {
		inputs := make([]string, 0, len(parts))
		holdMs := 0

		for _, part := range parts {
			input, duration, err := parseInput(part)
			if err != nil {
				return MacroStep{}, err
			}

			inputs = append(inputs, input)
			if duration > holdMs {
				holdMs = duration
			}
		}

		return MacroStep{
			Inputs: inputs,
			HoldMs: holdMs,
		}, nil
	}

	input, duration, err := parseInput(token)
	if err != nil {
		return MacroStep{}, err
	}

	return MacroStep{
		Inputs: []string{input},
		HoldMs: duration,
	}, nil
}

func parseInput(token string) (string, int, error) {
	token = strings.TrimSpace(token)
	if token == "" {
		return "", 0, fmt.Errorf("empty input token")
	}

	inputName := token
	holdMs := 0

	if idx := strings.IndexAny(token, ":@"); idx >= 0 {
		inputName = strings.TrimSpace(token[:idx])
		rest := strings.TrimSpace(token[idx+1:])

		if rest != "" {
			var err error
			holdMs, err = parseDuration(rest)
			if err != nil {
				return "", 0, fmt.Errorf("invalid duration %q: %w", rest, err)
			}
		}
	}

	normalized, ok := aliases[strings.ToLower(inputName)]
	if !ok {
		return "", 0, fmt.Errorf("unsupported input %q", inputName)
	}

	return normalized, holdMs, nil
}

func parseDuration(raw string) (int, error) {
	trimmed := strings.TrimSpace(strings.ToLower(raw))
	if trimmed == "" {
		return 0, nil
	}

	if strings.HasSuffix(trimmed, "ms") {
		value := strings.TrimSpace(strings.TrimSuffix(trimmed, "ms"))
		return strconv.Atoi(value)
	}

	if strings.HasSuffix(trimmed, "s") {
		value := strings.TrimSpace(strings.TrimSuffix(trimmed, "s"))
		seconds, err := strconv.Atoi(value)
		if err != nil {
			return 0, err
		}
		return seconds * 1000, nil
	}

	return strconv.Atoi(trimmed)
}

var aliases = map[string]string{
	"a": "A",
	"b": "B",
	"x": "X",
	"y": "Y",

	"lb":          "LB",
	"rb":          "RB",
	"leftbumper":  "LB",
	"rightbumper": "RB",

	"lt":           "LT",
	"rt":           "RT",
	"lefttrigger":  "LT",
	"righttrigger": "RT",

	"up":    "UP",
	"down":  "DOWN",
	"left":  "LEFT",
	"right": "RIGHT",

	"start": "START",
	"menu":  "START",

	"back": "BACK",
	"view": "BACK",

	"ls":         "LS",
	"rs":         "RS",
	"leftstick":  "LS",
	"rightstick": "RS",
}