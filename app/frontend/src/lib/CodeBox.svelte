<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	import { oneDark } from "@codemirror/theme-one-dark";
	import { node_hover, xml_schema } from "./CodeBox_Constants";

	let value = "";
</script>

<CodeMirror
	class="text-left h-full flex-auto font-mono text-lg font-bold"
	bind:value
	lang={xml(xml_schema)}
	tabSize={4}
	theme={oneDark}
	extensions={[node_hover]}
/>

<style>
	.cm-editor {
		height: 100% !important;
	}
	.cm-scroller {
		height: 100% !important;
	}
</style>
