<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	import { oneDark } from "@codemirror/theme-one-dark";
	import { hoverTooltip } from "@codemirror/view";

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

	const node_information_list = new Map<string, string>([
		["Root", "Start of every node tree"],
		["Selector", "Runs children until one succeeds"],
		["Sequence", "Runs children until one fails"],
		["Invert", "Inverts the result of the child"],
		["Repeat", "Repeats the child until it fails"],
		["Condition", "Checks if the condition is true"],
		["Fail", "Always fails"],
		["Succeed", "Always succeeds"],
		["ToRoot", "Goes to the given root node id"],
		["Turn", "Turns the car"],
		["Move", "Moves the car"],
		["Direction", "Sets the direction of the car"],
		["Repeat", "Repeats the child given the amount of times"],
		["Task", "Runs its action nodes in order"],
		["Action", "An action to run"],
	]);

	// Modified code from: https://codemirror.net/examples/tooltip/
	const node_hover = hoverTooltip((view, pos, side) => {
		let { from, to, text } = view.state.doc.lineAt(pos);
		let start = pos,
			end = pos;
		while (start > from && /\w/.test(text[start - from - 1])) start--;
		while (end < to && /\w/.test(text[end - from])) end++;
		if ((start == pos && side < 0) || (end == pos && side > 0)) return null;
		const highlighted_node = text.slice(start - from, end - from);
		const node_information = node_information_list.get(highlighted_node);
		if (!node_information) return null;
		return {
			pos: start,
			end,
			above: true,
			create(view) {
				let dom = document.createElement("div");
				dom.textContent = node_information;
				return { dom };
			},
		};
	});
</script>

<div style="height:50vh;" class="w-full text-left flex-auto">
	<CodeMirror
		class="h-full flex-auto font-mono text-lg font-bold"
		bind:value
		lang={xml(schema)}
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
