package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"strings"
	"time"

	"xbox_macros/internal/engine"
)

func main() {
	bindings, err := engine.LoadBindings("profiles/test.json")
	if err != nil {
		fmt.Fprintf(os.Stderr, "error loading bindings: %v\n", err)
		os.Exit(1)
	}

	reader := bufio.NewReader(os.Stdin)

	armed := false
	var triggerTime time.Time

	fmt.Println("Listening...")

	for {
		fmt.Print("> ")

		line, err := reader.ReadString('\n')
		if err != nil {
			fmt.Fprintf(os.Stderr, "read error: %v\n", err)
			continue
		}

		raw := strings.TrimSpace(line)

		if raw == "" {
			continue
		}

		fmt.Println("Received:", raw)

		input, err := engine.ParseMacro(raw)
		if err != nil {
			fmt.Println("Parse error:", err)
			continue
		}

		fmt.Printf("Parsed: %+v\n", input)

		if armed {
			if time.Since(triggerTime) > 10*time.Second {
				fmt.Println("Trigger expired.")
				armed = false
				continue
			}

			binding, ok := bindings.MatchMacro(input)
			if !ok {
				fmt.Println("No macro match.")
				continue
			}

			fmt.Println("Matched:", binding.Name)

			encoder := json.NewEncoder(os.Stdout)
			encoder.SetIndent("", "  ")
			encoder.Encode(binding.Output)

			armed = false
			continue
		}

		if bindings.MatchTrigger(input) {
			armed = true
			triggerTime = time.Now()
			fmt.Println("Trigger Activated.")
			continue
		}

		fmt.Println("No trigger match.")
	}
}