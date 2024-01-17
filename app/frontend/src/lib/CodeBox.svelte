<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	let value = "";

	// From @codemirror/lang-xml repository
	let schema1 = {
		elements: [
			{
				name: "doc",
				top: true,
				attributes: [
					"attr1",
					"attr2",
					{
						name: "attr3",
						values: ["x", "y"],
						completion: { type: "keyword" },
					},
				],
				children: ["head", "body"],
				completion: { type: "keyword" },
			},
			{ name: "head", attributes: ["attr2"] },
			{ name: "body", children: [] },
		],
		attributes: [
			{ name: "attr2", values: ["one", "two"] },
			{ name: "attrglobal", global: true },
		],
	};
</script>

<div class="text-left">
	<CodeMirror class="w-60 h-60" bind:value lang={xml(schema1)} />
</div>
