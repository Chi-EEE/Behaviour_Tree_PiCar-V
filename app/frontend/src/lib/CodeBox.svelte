<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	import { oneDark } from "@codemirror/theme-one-dark";
	let value = "";

	let common_children = [
		// Composite
		"Selector",
		"Sequence",

		// Decorator
		"Invert",
		"Repeat",

		// Leaf
		"Condition",
		"Fail",
		"LogMessage",
		"Succeed",
		"Task",
		"ToRoot",
		"Wait",
	];
	let schema = {
		elements: [
			{
				name: "Root",
				top: true,
				attributes: ["id"],
				children: common_children,
				completion: { type: "keyword" },
			},
			// Composite
			{
				name: "Selector",
				children: common_children,
				completion: { type: "keyword" },
			},
			{
				name: "Sequence",
				children: common_children,
				completion: { type: "keyword" },
			},

			// Decorator
			{
				name: "Invert",
				children: common_children,
				completion: { type: "keyword" },
			},
			{
				name: "Repeat",
				children: common_children,
				completion: { type: "keyword" },
			},

			// Leaf
			{
				name: "Condition",
				attributes: [
					{
						name: "type",
						values: ["NearbyPoints"],
						completion: { type: "keyword" },
					},
					"min_angle",
					"max_angle",
					"distance",
				],
				completion: { type: "keyword" },
			},
			{
				name: "Fail",
				completion: { type: "keyword" },
			},
			{
				name: "Succeed",
				completion: { type: "keyword" },
			},
			{
				name: "ToRoot",
				attributes: ["id"],
				completion: { type: "keyword" },
			},
			{
				name: "Task",
				children: ["Action"],
				completion: { type: "keyword" },
			},
			{
				name: "Action",
				attributes: [
					{
						name: "type",
						values: ["Turn", "Move", "Direction"],
						completion: { type: "keyword" },
					},
					"angle",
					"ms",
					{
						name: "direction",
						values: ["forward"],
						completion: { type: "keyword" },
					},
				],
				completion: { type: "keyword" },
			},
		],
		attributes: [{ name: "name", global: true }],
	};
</script>

<div style="height:50vh;" class="w-full text-left flex-auto">
	<CodeMirror
		class="h-full flex-auto font-mono text-lg font-bold"
		bind:value
		lang={xml(schema)}
		theme={oneDark}
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
