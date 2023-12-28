package main

import (
	"fmt"
	"io/ioutil"

	wasmer "github.com/wasmerio/wasmer-go/wasmer"
)

func main() {
	wasmBytes, err := ioutil.ReadFile("../build/roaster1.wasm")
	check(err)

	store := wasmer.NewStore(wasmer.NewEngine())
	module, err := wasmer.NewModule(store, wasmBytes)
	check(err)

	wasiEnv, _ := wasmer.NewWasiStateBuilder("wasi-program").
		// Choose according to your actual situation
		// Argument("--foo").
		// Environment("ABC", "DEF").
		// MapDirectory("./", ".").
		Finalize()
	importObject, err := wasiEnv.GenerateImportObject(store, module)
	check(err)

	importObject.Register("env", map[string]wasmer.IntoExtern{
		"emscripten_notify_memory_growth": wasmer.NewFunction(
			store,
			wasmer.NewFunctionType(wasmer.NewValueTypes(wasmer.I32), wasmer.NewValueTypes()),
			func(v []wasmer.Value) ([]wasmer.Value, error) {
				fmt.Println("memory growth!")
				return nil, nil
			},
		),
	})

	instance, err := wasmer.NewInstance(module, importObject)
	check(err)

	start, err := instance.Exports.GetWasiStartFunction()
	check(err)
	start()

	mainFn, err := instance.Exports.GetFunction("main")
	check(err)
	result, _ := mainFn()
	fmt.Println(result)
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}
