<script lang="ts">
	// If this does not work then try preview
	import CodeMirror from "svelte-codemirror-editor";
	import { xml } from "@codemirror/lang-xml";
	import { oneDark } from "@codemirror/theme-one-dark";
	import { hoverTooltip } from "@codemirror/view";
	import { range_0_100, range_0_180 } from "./numbers";

	let value = "";
	const common_children = [
		// Composite
		"Selector",
		"Sequence",

		// Decorator
		"Invert",
		"Repeat",

		// Leaf
		"Fail",
		"LogMessage",
		"Succeed",
		"Task",
		"ToRoot",
		"Wait",

		// Conditions
		"Condition:NearbyPoints",
	];

	const task_nodes = [
		"Action:Turn",
		"Action:Move",
		"Action:Direction",
		"Condition:NearbyPoints",
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
				children: task_nodes,
				completion: { type: "keyword" },
			},
			{
				name: "Action:Turn",
				attributes: [
					{
						name: "angle",
						values: range_0_180,
						completion: { type: "keyword" },
					},
				],
				completion: { type: "keyword" },
			},
			{
				name: "Action:Move",
				attributes: [
					{
						name: "speed",
						values: range_0_100,
						completion: { type: "keyword" },
					},
				],
				completion: { type: "keyword" },
			},
			{
				name: "Action:Direction",
				attributes: [
					{
						name: "direction",
						values: ["Forward", "Backward"],
						completion: { type: "keyword" },
					},
				],
				completion: { type: "keyword" },
			},
			{
				name: "Condition:NearbyPoints",
				attributes: [
					{
						name: "min_angle",
						values: range_0_180,
						completion: { type: "keyword" },
					},
					{
						name: "max_angle",
						values: range_0_180,
						completion: { type: "keyword" },
					},
					"distance",
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
		["Fail", "Always fails"],
		["Succeed", "Always succeeds"],
		["ToRoot", "Goes to the given root node id"],
		["Repeat", "Repeats the child given the amount of times"],

		["LogMessage", "Logs the given message"],
		["Wait", "Waits for the given amount of time"],

		["Task", "Runs its action nodes in order"],

		["Action:Turn", "Turns the car by the given angle"],
		["Action:Move", "Moves the car by the given speed"],
		["Action:Direction", "Sets the direction of the car"],
		["Action:Stop", "Stops the car from moving"],

		["Condition:NearbyPoints", "Checks if there are nearby points"],
	]);

	// Modified code from: https://codemirror.net/examples/tooltip/
	const node_hover = hoverTooltip((view, pos, side) => {
		let { from, to, text } = view.state.doc.lineAt(pos);
		let start = pos,
			end = pos;
		while (start > from && /[\w|:]/.test(text[start - from - 1])) start--;
		while (end < to && /[\w|:]/.test(text[end - from])) end++;
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
