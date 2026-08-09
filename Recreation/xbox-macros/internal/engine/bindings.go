package engine

import (
	"encoding/json"
	"fmt"
	"os"
)

type Binding struct {
	Name      string `json:"name"`
	MacroRaw  string `json:"macro"`
	OutputRaw string `json:"output"`

	Macro  Macro `json:"-"`
	Output Macro `json:"-"`
}

type BindingConfig struct {
	TriggerRaw string    `json:"trigger"`
	Bindings   []Binding `json:"bindings"`

	Trigger Macro `json:"-"`
}

func LoadBindings(path string) (*BindingConfig, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}

	var cfg BindingConfig
	if err := json.Unmarshal(data, &cfg); err != nil {
		return nil, err
	}

	trigger, err := ParseMacro(cfg.TriggerRaw)
	if err != nil {
		return nil, fmt.Errorf("trigger: %w", err)
	}

	cfg.Trigger = trigger

	for i := range cfg.Bindings {
		macro, err := ParseMacro(cfg.Bindings[i].MacroRaw)
		if err != nil {
			return nil, fmt.Errorf("binding %d macro: %w", i, err)
		}

		output, err := ParseMacro(cfg.Bindings[i].OutputRaw)
		if err != nil {
			return nil, fmt.Errorf("binding %d output: %w", i, err)
		}

		cfg.Bindings[i].Macro = macro
		cfg.Bindings[i].Output = output
	}

	return &cfg, nil
}

func (cfg *BindingConfig) MatchTrigger(input Macro) bool {
	return macrosEqual(input, cfg.Trigger)
}

func (cfg *BindingConfig) MatchMacro(input Macro) (*Binding, bool) {
	for i := range cfg.Bindings {
		if macrosEqual(input, cfg.Bindings[i].Macro) {
			return &cfg.Bindings[i], true
		}
	}

	return nil, false
}

func macrosEqual(a, b Macro) bool {
	if len(a.Steps) != len(b.Steps) {
		return false
	}

	for i := range a.Steps {
		if !stepsEqual(a.Steps[i], b.Steps[i]) {
			return false
		}
	}

	return true
}

func stepsEqual(a, b MacroStep) bool {
	if a.HoldMs != b.HoldMs || a.DelayMs != b.DelayMs {
		return false
	}

	if len(a.Inputs) != len(b.Inputs) {
		return false
	}

	for i := range a.Inputs {
		if a.Inputs[i] != b.Inputs[i] {
			return false
		}
	}

	return true
}