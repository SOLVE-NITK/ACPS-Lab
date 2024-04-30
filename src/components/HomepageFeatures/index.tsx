import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  Svg: React.ComponentType<React.ComponentProps<'svg'>>;
  description: JSX.Element;
};

const FeatureList: FeatureItem[] = [
  {
    title: 'Experiential Learning',
    Svg: require('@site/static/img/undraw_docusaurus_mountain.svg').default,
    description: (
      <>
      Our framework enhances learning through hands-on experience in Cyber-Physical Systems (CPS), merging disciplines like engineering and computer science to solve complex problems and prepare students for technological careers
      </>
    ),
  },
  {
    title: 'Innovation and Research',
    Svg: require('@site/static/img/undraw_docusaurus_tree.svg').default,
    description: (
      <>
      We enable groundbreaking research in CPS, allowing users to develop and test innovative technologies that push the boundaries of automation and system integration, leading to potential commercial advancements.
      </>
    ),
  },
  {
    title: 'Significance',
    Svg: require('@site/static/img/undraw_docusaurus_react.svg').default,
    description: (
      <>
      Our project is is crucial for advancing global industries by equipping future leaders with the skills to innovate and improve systems like manufacturing and autonomous vehicles, fostering a smarter, more connected world.
      </>
    ),
  },
];

function Feature({title, Svg, description}: FeatureItem) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <Svg className={styles.featureSvg} role="img" />
      </div>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): JSX.Element {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
