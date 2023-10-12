const cars = {
	"1": 'Inital Version',
};

export function load({ params }) {
	return {
		car_name: cars[params.car_name]
	};
}
