<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	import { oneDark } from "@codemirror/theme-one-dark";
	import { node_hover, xml_schema } from "./CodeBox_Constants";
	import xmlFormat from 'xml-formatter';

	export let websocket: WebSocket;

	let xml_code: string = "";

	function sendXMLCode() {
		websocket.send(JSON.stringify({ type: "behaviour_tree", data: xmlFormat.minify(xml_code) }));
	}
</script>

<button on:mousedown={sendXMLCode}>Click Here!</button>
<div class="h-full">
	<CodeMirror
		class="text-left h-full flex-auto font-mono text-lg font-bold"
		bind:value={xml_code}
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
</div>
